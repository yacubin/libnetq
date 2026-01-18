/*
 * MIT License
 *
 * Copyright (c) 2025-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/Leb128.h"

#include <libnetq/Assert.h>
#include <libnetq/BitOps.h>
#include <libnetq/Math.h>
#include <libnetq/ConstExpr.h>

#define CHUNK_BITS 7

#define bitsOfIndex(index) (CHUNK_BITS * (index))
#define equalLength(buffer, length) ((((const uint8_t*)(buffer))[length - 1] & 0x80) == 0)
#define isNegative(buffer, length) (((const uint8_t*)(buffer))[(length) - 1] & 0x40)

#define negativeValueExtend(type, length) \
  ((type)(~(0ULL) << (NQGetMin(CHUNK_BITS * (length), sizeof(type) * 8) - 1)))

#define getByte(value, index) \
  ((value) >> bitsOfIndex(index))

#define getValue(type, buffer, index) \
  ((type)((const uint8_t*)buffer)[index] << bitsOfIndex(index))

#define readPositiveChunk(type, result, buffer, length) \
  *(result) = getValue(type, buffer, (length) - 1)

#define readNegativeChunk(type, result, buffer, length) \
  *(result) = getValue(type, buffer, (length) - 1) | negativeValueExtend(type, length)

#define readSequenceChunk(type, result, buffer, index) \
  *(result) |= ((type)(((const uint8_t*)buffer)[index] & 0x7f) << bitsOfIndex(index))

#define readSequenceData1(type, result, buffer)
#define readSequenceData2(type, result, buffer) readSequenceChunk(type, result, buffer, 0)
#define readSequenceData3(type, result, buffer) readSequenceChunk(type, result, buffer, 1); readSequenceData2(type, result, buffer)
#define readSequenceData4(type, result, buffer) readSequenceChunk(type, result, buffer, 2); readSequenceData3(type, result, buffer)
#define readSequenceData5(type, result, buffer) readSequenceChunk(type, result, buffer, 3); readSequenceData4(type, result, buffer)
#define readSequenceData6(type, result, buffer) readSequenceChunk(type, result, buffer, 4); readSequenceData5(type, result, buffer)
#define readSequenceData7(type, result, buffer) readSequenceChunk(type, result, buffer, 5); readSequenceData6(type, result, buffer)
#define readSequenceData8(type, result, buffer) readSequenceChunk(type, result, buffer, 6); readSequenceData7(type, result, buffer)
#define readSequenceData9(type, result, buffer) readSequenceChunk(type, result, buffer, 7); readSequenceData8(type, result, buffer)
#define readSequenceData10(type, result, buffer) readSequenceChunk(type, result, buffer, 8); readSequenceData9(type, result, buffer)

#define writeSequenceChunk(buffer, index, value) \
  ((uint8_t*)(buffer))[index] = (uint8_t)(getByte(value, index) | 0x80)

#define writePositiveChunk(buffer, index, value) \
  ((uint8_t*)(buffer))[index] = (uint8_t)getByte(value, index)

#define writeNegativeChunk(buffer, index, value) \
  ((uint8_t*)(buffer))[index] = (uint8_t)(getByte(value, index) & 0x7f)

#define writeSequenceData1(buffer, value)
#define writeSequenceData2(buffer, value) writeSequenceChunk(buffer, 0, value)
#define writeSequenceData3(buffer, value) writeSequenceChunk(buffer, 1, value); writeSequenceData2(buffer, value)
#define writeSequenceData4(buffer, value) writeSequenceChunk(buffer, 2, value); writeSequenceData3(buffer, value)
#define writeSequenceData5(buffer, value) writeSequenceChunk(buffer, 3, value); writeSequenceData4(buffer, value)
#define writeSequenceData6(buffer, value) writeSequenceChunk(buffer, 4, value); writeSequenceData5(buffer, value)
#define writeSequenceData7(buffer, value) writeSequenceChunk(buffer, 5, value); writeSequenceData6(buffer, value)
#define writeSequenceData8(buffer, value) writeSequenceChunk(buffer, 6, value); writeSequenceData7(buffer, value)
#define writeSequenceData9(buffer, value) writeSequenceChunk(buffer, 7, value); writeSequenceData8(buffer, value)
#define writeSequenceData10(buffer, value) writeSequenceChunk(buffer, 8, value); writeSequenceData9(buffer, value)

#define LEB128_U1_MAX  ((NQ_UINT32_C(1) << bitsOfIndex(1)) - 1)
#define LEB128_U2_MAX  ((NQ_UINT32_C(1) << bitsOfIndex(2)) - 1)
#define LEB128_U3_MAX  ((NQ_UINT32_C(1) << bitsOfIndex(3)) - 1)
#define LEB128_U4_MAX  ((NQ_UINT32_C(1) << bitsOfIndex(4)) - 1)
#define LEB128_U5_MAX  ((NQ_UINT64_C(1) << bitsOfIndex(5)) - 1)
#define LEB128_U6_MAX  ((NQ_UINT64_C(1) << bitsOfIndex(6)) - 1)
#define LEB128_U7_MAX  ((NQ_UINT64_C(1) << bitsOfIndex(7)) - 1)
#define LEB128_U8_MAX  ((NQ_UINT64_C(1) << bitsOfIndex(8)) - 1)
#define LEB128_U9_MAX  ((NQ_UINT64_C(1) << bitsOfIndex(9)) - 1)
#define LEB128_U10_MAX ((NQ_UINT64_C(1) << bitsOfIndex(10)) - 1)

#define LEB128_S1_MAX  ((int32_t)(LEB128_U1_MAX >> 1))
#define LEB128_S1_MIN  (-LEB128_S1_MAX - 1)
#define LEB128_S2_MAX  ((int32_t)(LEB128_U2_MAX >> 1))
#define LEB128_S2_MIN  (-LEB128_S2_MAX - 1)
#define LEB128_S3_MAX  ((int32_t)(LEB128_U3_MAX >> 1))
#define LEB128_S3_MIN  (-LEB128_S3_MAX - 1)
#define LEB128_S4_MAX  ((int32_t)(LEB128_U4_MAX >> 1))
#define LEB128_S4_MIN  (-LEB128_S4_MAX - 1)
#define LEB128_S5_MAX  ((int64_t)(LEB128_U5_MAX >> 1))
#define LEB128_S5_MIN  (-LEB128_S5_MAX - 1)
#define LEB128_S6_MAX  ((int64_t)(LEB128_U6_MAX >> 1))
#define LEB128_S6_MIN  (-LEB128_S6_MAX - 1)
#define LEB128_S7_MAX  ((int64_t)(LEB128_U7_MAX >> 1))
#define LEB128_S7_MIN  (-LEB128_S7_MAX - 1)
#define LEB128_S8_MAX  ((int64_t)(LEB128_U8_MAX >> 1))
#define LEB128_S8_MIN  (-LEB128_S8_MAX - 1)
#define LEB128_S9_MAX  ((int64_t)(LEB128_U9_MAX >> 1))
#define LEB128_S9_MIN  (-LEB128_S9_MAX - 1)
#define LEB128_S10_MAX  ((int64_t)(LEB128_U10_MAX >> 1))
#define LEB128_S10_MIN  (-LEB128_S10_MAX - 1)

#define readPositiveSafe(type, result, buffer, length) \
  if (result) { \
    readPositiveChunk(type, result, buffer, length); \
    readSequenceData##length(type, result, buffer); \
  }

#define readNegativeSafe(type, result, buffer, length) \
  if (result) { \
    readNegativeChunk(type, result, buffer, length); \
    readSequenceData##length(type, result, buffer); \
  }

#define readSignedSafe(type, result, buffer, length) \
  if (result) { \
    if (isNegative(data, length)) \
      readNegativeChunk(type, result, buffer, length); \
    else \
      readPositiveChunk(type, result, buffer, length); \
    readSequenceData##length(type, result, buffer); \
  }

#define writePositiveSafe(buffer, length, value) \
  if (buffer) { \
    writePositiveChunk(buffer, (length) - 1, value); \
    writeSequenceData##length(buffer, value); \
  }

#define writeNegativeSafe(buffer, length, value) \
  if (buffer) { \
    writeNegativeChunk(buffer, (length) - 1, value); \
    writeSequenceData##length(buffer, value); \
  }

size_t NQLeb128EncodeInt8(void* buffer, size_t size, int8_t value)
{
  if (value < 0) {
    if (LEB128_S1_MIN <= value) {
      if (size >= 1)
        writeNegativeSafe(buffer, 1, value);
      return 1;
    }

    if (size >= 2)
      writeNegativeSafe(buffer, 2, value);
    return 2;
  }

  if (value <= LEB128_S1_MAX) {
    if (size >= 1)
      writePositiveSafe(buffer, 1, value);
    return 1;
  }

  if (size >= 2)
    writePositiveSafe(buffer, 2, value);
  return 2;
}

size_t NQLeb128EncodeInt16(void* buffer, size_t size, int16_t value)
{
  if (value < 0) {
    if (LEB128_S1_MIN <= value) {
      if (size >= 1)
        writeNegativeSafe(buffer, 1, value);
      return 1;
    }

    if (LEB128_S2_MIN <= value) {
      if (size >= 2)
        writeNegativeSafe(buffer, 2, value);
      return 2;
    }

    if (size >= 3)
      writeNegativeSafe(buffer, 3, value);
    return 3;
  }

  if (value <= LEB128_S1_MAX) {
    if (size >= 1)
      writePositiveSafe(buffer, 1, value);
    return 1;
  }

  if (value <= LEB128_S2_MAX) {
    if (size >= 2)
      writePositiveSafe(buffer, 2, value);
    return 2;
  }

  if (size >= 3)
    writePositiveSafe(buffer, 3, value);
  return 3;
}

size_t NQLeb128EncodeInt32(void* buffer, size_t size, int32_t value)
{
  if (value < 0) {
    if (LEB128_S1_MIN <= value) {
      if (size >= 1)
        writeNegativeSafe(buffer, 1, value);
      return 1;
    }

    if (LEB128_S2_MIN <= value) {
      if (size >= 2)
        writeNegativeSafe(buffer, 2, value);
      return 2;
    }

    if (LEB128_S3_MIN <= value) {
      if (size >= 3)
        writeNegativeSafe(buffer, 3, value);
      return 3;
    }

    if (LEB128_S4_MIN <= value) {
      if (size >= 4)
        writeNegativeSafe(buffer, 4, value);
      return 4;
    }

    if (size >= 5)
      writeNegativeSafe(buffer, 5, value);
    return 5;
  }

  if (value <= LEB128_S1_MAX) {
    if (size >= 1)
      writePositiveSafe(buffer, 1, value);
    return 1;
  }

  if (value <= LEB128_S2_MAX) {
    if (size >= 2)
      writePositiveSafe(buffer, 2, value);
    return 2;
  }

  if (value <= LEB128_S3_MAX) {
    if (size >= 3)
      writePositiveSafe(buffer, 3, value);
    return 3;
  }

  if (value <= LEB128_S4_MAX) {
    if (size >= 4)
      writePositiveSafe(buffer, 4, value);
    return 4;
  }

  if (size >= 5)
    writePositiveSafe(buffer, 5, value);
  return 5;
}

size_t NQLeb128EncodeInt64(void* buffer, size_t size, int64_t value)
{
  if (value < 0) {
    if (LEB128_S1_MIN <= value) {
      if (size >= 1)
        writeNegativeSafe(buffer, 1, value);
      return 1;
    }
    if (LEB128_S2_MIN <= value) {
      if (size >= 2)
        writeNegativeSafe(buffer, 2, value);
      return 2;
    }
    if (LEB128_S3_MIN <= value) {
      if (size >= 3)
        writeNegativeSafe(buffer, 3, value);
      return 3;
    }
    if (LEB128_S4_MIN <= value) {
      if (size >= 4)
        writeNegativeSafe(buffer, 4, value);
      return 4;
    }
    if (LEB128_S5_MIN <= value) {
      if (size >= 5)
        writeNegativeSafe(buffer, 5, value);
      return 5;
    }
    if (LEB128_S6_MIN <= value) {
      if (size >= 6)
        writeNegativeSafe(buffer, 6, value);
      return 6;
    }
    if (LEB128_S7_MIN <= value) {
      if (size >= 7)
        writeNegativeSafe(buffer, 7, value);
      return 7;
    }
    if (LEB128_S8_MIN <= value) {
      if (size >= 8)
        writeNegativeSafe(buffer, 8, value);
      return 8;
    }
    if (LEB128_S9_MIN <= value) {
      if (size >= 9)
        writeNegativeSafe(buffer, 9, value);
      return 9;
    }
    if (size >= 10) {
      writeNegativeSafe(buffer, 10, value);
    }
    return 10;
  }

  if (value <= LEB128_S1_MAX) {
    if (size >= 1)
      writePositiveSafe(buffer, 1, value);
    return 1;
  }
  if (value <= LEB128_S2_MAX) {
    if (size >= 2)
      writePositiveSafe(buffer, 2, value);
    return 2;
  }
  if (value <= LEB128_S3_MAX) {
    if (size >= 3)
      writePositiveSafe(buffer, 3, value);
    return 3;
  }
  if (value <= LEB128_S4_MAX) {
    if (size >= 4)
      writePositiveSafe(buffer, 4, value);
    return 4;
  }
  if (value <= LEB128_S5_MAX) {
    if (size >= 5)
      writePositiveSafe(buffer, 5, value);
    return 5;
  }
  if (value <= LEB128_S6_MAX) {
    if (size >= 6)
      writePositiveSafe(buffer, 6, value);
    return 6;
  }
  if (value <= LEB128_S7_MAX) {
    if (size >= 7)
      writePositiveSafe(buffer, 7, value);
    return 7;
  }
  if (value <= LEB128_S8_MAX) {
    if (size >= 8)
      writePositiveSafe(buffer, 8, value);
    return 8;
  }
  if (value <= LEB128_S9_MAX) {
    if (size >= 9)
      writePositiveSafe(buffer, 9, value);
    return 9;
  }
  if (size >= 10)
    writePositiveSafe(buffer, 10, value);
  return 10;
}

size_t NQLeb128EncodeUint8(void* buffer, size_t size, uint8_t value)
{
  if (value <= LEB128_U1_MAX) {
    if (size >= 1)
      writePositiveSafe(buffer, 1, value);
    return 1;
  }

  if (size >= 2)
    writePositiveSafe(buffer, 2, value);
  return 2;
}

size_t NQLeb128EncodeUint16(void* buffer, size_t size, uint16_t value)
{
  if (value <= LEB128_U1_MAX) {
    if (size >= 1)
      writePositiveSafe(buffer, 1, value);
    return 1;
  }

  if (value <= LEB128_U2_MAX) {
    if (size >= 2)
      writePositiveSafe(buffer, 2, value);
    return 2;
  }

  if (size >= 3)
    writePositiveSafe(buffer, 3, value);
  return 3;
}

size_t NQLeb128EncodeUint32(void* buffer, size_t size, uint32_t value)
{
  if (value <= LEB128_U1_MAX) {
    if (size >= 1)
      writePositiveSafe(buffer, 1, value);
    return 1;
  }

  if (value <= LEB128_U2_MAX) {
    if (size >= 2)
      writePositiveSafe(buffer, 2, value);
    return 2;
  }

  if (value <= LEB128_U3_MAX) {
    if (size >= 3)
      writePositiveSafe(buffer, 3, value);
    return 3;
  }

  if (value <= LEB128_U4_MAX) {
    if (size >= 4)
      writePositiveSafe(buffer, 4, value);
    return 4;
  }

  if (size >= 5)
    writePositiveSafe(buffer, 5, value);
  return 5;
}

size_t NQLeb128EncodeUint64(void* buffer, size_t size, uint64_t value)
{
  if (value <= LEB128_U1_MAX) {
    if (size >= 1)
      writePositiveSafe(buffer, 1, value);
    return 1;
  }

  if (value <= LEB128_U2_MAX) {
    if (size >= 2)
      writePositiveSafe(buffer, 2, value);
    return 2;
  }

  if (value <= LEB128_U3_MAX) {
    if (size >= 3)
      writePositiveSafe(buffer, 3, value);
    return 3;
  }

  if (value <= LEB128_U4_MAX) {
    if (size >= 4)
      writePositiveSafe(buffer, 4, value);
    return 4;
  }

  if (value <= LEB128_U5_MAX) {
    if (size >= 5)
      writePositiveSafe(buffer, 5, value);
    return 5;
  }

  if (value <= LEB128_U6_MAX) {
    if (size >= 6)
      writePositiveSafe(buffer, 6, value);
    return 6;
  }

  if (value <= LEB128_U7_MAX) {
    if (size >= 7)
      writePositiveSafe(buffer, 7, value);
    return 7;
  }

  if (value <= LEB128_U8_MAX) {
    if (size >= 8)
      writePositiveSafe(buffer, 8, value);
    return 8;
  }

  if (value <= LEB128_U9_MAX) {
    if (size >= 9)
      writePositiveSafe(buffer, 9, value);
    return 9;
  }

  if (size >= 10)
    writePositiveSafe(buffer, 10, value);
  return 10;
}

size_t NQLeb128DecodeInt8(const void* data, size_t size, int8_t* result)
{
  if (size < 1)
    return 0;

  if (equalLength(data, 1)) {
    readSignedSafe(int8_t, result, data, 1);
    return 1;
  }

  uint8_t b1 = ((const uint8_t*)(data))[1];
  if (b1 == 0x7f) {
    readNegativeSafe(int8_t, result, data, 2);
    return 2;
  }
  if (b1 == 0x00) {
    readPositiveSafe(int8_t, result, data, 2);
    return 2;
  }

  return 0;
}

size_t NQLeb128DecodeInt16(const void* data, size_t size, int16_t* result)
{
  if (size < 1)
    return 0;

  if (equalLength(data, 1)) {
    readSignedSafe(int16_t, result, data, 1);
    return 1;
  }

  if (size < 2)
    return 0;

  if (equalLength(data, 2)) {
    readSignedSafe(int16_t, result, data, 2);
    return 2;
  }

  if (size < 3)
    return 0;

  uint8_t b2 = ((const uint8_t*)(data))[2] & 0xfe;
  if (b2 == 0x7e) {
    readNegativeSafe(int16_t, result, data, 3);
    return 3;
  }
  if (b2 == 0x00) {
    readPositiveSafe(int16_t, result, data, 3);
    return 3;
  }

  return 0;
}

size_t NQLeb128DecodeInt32(const void* data, size_t size, int32_t* result)
{
  if (size < 1)
    return 0;

  if (equalLength(data, 1)) {
    readSignedSafe(int32_t, result, data, 1);
    return 1;
  }

  if (size < 2)
    return 0;

  if (equalLength(data, 2)) {
    readSignedSafe(int32_t, result, data, 2);
    return 2;
  }

  if (size < 3)
    return 0;

  if (equalLength(data, 3)) {
    readSignedSafe(int32_t, result, data, 3);
    return 3;
  }

  if (size < 4)
    return 0;

  if (equalLength(data, 4)) {
    readSignedSafe(int32_t, result, data, 4);
    return 4;
  }

  if (size < 5)
    return 0;

  uint8_t b4 = ((const uint8_t*)(data))[4] & 0xf8;
  if (b4 == 0x78) {
    readNegativeSafe(int32_t, result, data, 5);
    return 5;
  }
  if (b4 == 0x00) {
    readPositiveSafe(int32_t, result, data, 5);
    return 5;
  }

  return 0;
}

size_t NQLeb128DecodeInt64(const void* data, size_t size, int64_t* result)
{
  if (size < 1)
    return 0;

  if (equalLength(data, 1)) {
    readSignedSafe(int64_t, result, data, 1);
    return 1;
  }

  if (size < 2)
    return 0;

  if (equalLength(data, 2)) {
    readSignedSafe(int64_t, result, data, 2);
    return 2;
  }

  if (size < 3)
    return 0;

  if (equalLength(data, 3)) {
    readSignedSafe(int64_t, result, data, 3);
    return 3;
  }

  if (size < 4)
    return 0;

  if (equalLength(data, 4)) {
    readSignedSafe(int64_t, result, data, 4);
    return 4;
  }

  if (size < 5)
    return 0;

  if (equalLength(data, 5)) {
    readSignedSafe(int64_t, result, data, 5);
    return 5;
  }

  if (size < 6)
    return 0;

  if (equalLength(data, 6)) {
    readSignedSafe(int64_t, result, data, 6);
    return 6;
  }

  if (size < 7)
    return 0;

  if (equalLength(data, 7)) {
    readSignedSafe(int64_t, result, data, 7);
    return 7;
  }

  if (size < 8)
    return 0;

  if (equalLength(data, 8)) {
    readSignedSafe(int64_t, result, data, 8);
    return 8;
  }

  if (size < 9)
    return 0;

  if (equalLength(data, 9)) {
    readSignedSafe(int64_t, result, data, 9);
    return 9;
  }

  if (size < 10)
    return 0;

  uint8_t b9 = ((const uint8_t*)(data))[9];
  if (b9 == 0x7f) {
    readNegativeSafe(int64_t, result, data, 10);
    return 10;
  }
  if (b9 == 0x00) {
    readPositiveSafe(int64_t, result, data, 10);
    return 10;
  }

  return 0;
}

size_t NQLeb128DecodeUint8(const void* data, size_t size, uint8_t* result)
{
  if (size < 1)
    return 0;

  if (equalLength(data, 1)) {
    readPositiveSafe(uint8_t, result, data, 1);
    return 1;
  }

  if (size < 2)
    return 0;

  uint8_t b1 = ((const uint8_t*)(data))[1] & 0xfe;
  if (b1 == 0) {
    readPositiveSafe(uint8_t, result, data, 2);
    return 2;
  }

  return 0;
}

size_t NQLeb128DecodeUint16(const void* data, size_t size, uint16_t* result)
{
  if (size < 1)
    return 0;

  if (equalLength(data, 1)) {
    readPositiveSafe(uint16_t, result, data, 1);
    return 1;
  }

  if (size < 2)
    return 0;

  if (equalLength(data, 2)) {
    readPositiveSafe(uint16_t, result, data, 2);
    return 2;
  }

  if (size < 3)
    return 0;

  uint8_t b2 = ((const uint8_t*)(data))[2] & 0xfc;
  if (b2 == 0) {
    readPositiveSafe(uint16_t, result, data, 3);
    return 3;
  }

  return 0;
}

size_t NQLeb128DecodeUint32(const void* data, size_t size, uint32_t* result)
{
  if (size < 1)
    return 0;

  if (equalLength(data, 1)) {
    readPositiveSafe(uint32_t, result, data, 1);
    return 1;
  }

  if (size < 2)
    return 0;

  if (equalLength(data, 2)) {
    readPositiveSafe(uint32_t, result, data, 2);
    return 2;
  }

  if (size < 3)
    return 0;

  if (equalLength(data, 3)) {
    readPositiveSafe(uint32_t, result, data, 3);
    return 3;
  }

  if (size < 4)
    return 0;

  if (equalLength(data, 4)) {
    readPositiveSafe(uint32_t, result, data, 4);
    return 4;
  }

  if (size < 5)
    return 0;

  uint8_t b4 = ((const uint8_t*)(data))[4] & 0xf0;
  if (b4 == 0) {
    readPositiveSafe(uint32_t, result, data, 5);
    return 5;
  }

  return 0;
}

size_t NQLeb128DecodeUint64(const void* data, size_t size, uint64_t* result)
{
  if (size < 1)
    return 0;

  if (equalLength(data, 1)) {
    readPositiveSafe(uint64_t, result, data, 1);
    return 1;
  }

  if (size < 2)
    return 0;

  if (equalLength(data, 2)) {
    readPositiveSafe(uint64_t, result, data, 2);
    return 2;
  }

  if (size < 3)
    return 0;

  if (equalLength(data, 3)) {
    readPositiveSafe(uint64_t, result, data, 3);
    return 3;
  }

  if (size < 4)
    return 0;

  if (equalLength(data, 4)) {
    readPositiveSafe(uint64_t, result, data, 4);
    return 4;
  }

  if (size < 5)
    return 0;

  if (equalLength(data, 5)) {
    readPositiveSafe(uint64_t, result, data, 5);
    return 5;
  }

  if (size < 6)
    return 0;

  if (equalLength(data, 6)) {
    readPositiveSafe(uint64_t, result, data, 6);
    return 6;
  }

  if (size < 7)
    return 0;

  if (equalLength(data, 7)) {
    readPositiveSafe(uint64_t, result, data, 7);
    return 7;
  }

  if (size < 8)
    return 0;

  if (equalLength(data, 8)) {
    readPositiveSafe(uint64_t, result, data, 8);
    return 8;
  }

  if (size < 9)
    return 0;

  if (equalLength(data, 9)) {
    readPositiveSafe(uint64_t, result, data, 9);
    return 9;
  }

  if (size < 10)
    return 0;

  uint8_t b9 = ((const uint8_t*)(data))[9] & 0xfe;
  if (b9 == 0) {
    readPositiveSafe(uint64_t, result, data, 10);
    return 10;
  }

  return 0;
}

void NQLeb128Dec_init(NQLeb128Dec* thiz, bool isSigned)
{
  thiz->valueUint64 = 0;
  thiz->bitWidth = 0;
  thiz->isSigned = isSigned;
}

bool NQLeb128Dec_update(NQLeb128Dec* thiz, uint8_t byte)
{
  thiz->valueUint64 |= (NQ_UINT64_C(0x7f) & byte) << thiz->bitWidth;
  thiz->bitWidth += 7;

  if (byte & 0x80)
    return false;

  uint64_t highMask = (thiz->bitWidth >= 64) ? 0 : ~NQ_UINT64_C(0) << thiz->bitWidth;
  if (!thiz->isSigned) {
    thiz->valueUint64 &= ~highMask;
    thiz->bitWidth = NQGetMax(thiz->bitWidth - (NQGetClz32(byte) - 25), 1);
  }
  else {
    if ((byte & 0x40) == 0)
      thiz->valueUint64 &= ~highMask;
    else {
      byte = ~(byte | 0x80);
      thiz->valueUint64 |= highMask;
    }
    thiz->bitWidth = NQGetMax(thiz->bitWidth - (NQGetClz32(byte) - 26), 2);
  }

  return true;
}
