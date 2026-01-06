/*
 * MIT License
 *
 * Copyright (c) 2025-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_LEB128_H
#define _LIBNETQ_LEB128_H

#include <libnetq/Basic.h>
#include <libnetq/ConstExpr.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NQ_LEB128_BUFFER_SIZE (10)
typedef uint8_t NQLeb128Buffer[NQ_LEB128_BUFFER_SIZE];

NQ_EXPORT size_t NQLeb128EncodeInt8(void* buffer, size_t size, int8_t value);
NQ_EXPORT size_t NQLeb128EncodeInt16(void* buffer, size_t size, int16_t value);
NQ_EXPORT size_t NQLeb128EncodeInt32(void* buffer, size_t size, int32_t value);
NQ_EXPORT size_t NQLeb128EncodeInt64(void* buffer, size_t size, int64_t value);

NQ_EXPORT size_t NQLeb128EncodeUint8(void* buffer, size_t size, uint8_t value);
NQ_EXPORT size_t NQLeb128EncodeUint16(void* buffer, size_t size, uint16_t value);
NQ_EXPORT size_t NQLeb128EncodeUint32(void* buffer, size_t size, uint32_t value);
NQ_EXPORT size_t NQLeb128EncodeUint64(void* buffer, size_t size, uint64_t value);

NQ_EXPORT size_t NQLeb128DecodeInt8(const void* data, size_t size, int8_t* result);
NQ_EXPORT size_t NQLeb128DecodeInt16(const void* data, size_t size, int16_t* result);
NQ_EXPORT size_t NQLeb128DecodeInt32(const void* data, size_t size, int32_t* result);
NQ_EXPORT size_t NQLeb128DecodeInt64(const void* data, size_t size, int64_t* result);

NQ_EXPORT size_t NQLeb128DecodeUint8(const void* data, size_t size, uint8_t* result);
NQ_EXPORT size_t NQLeb128DecodeUint16(const void* data, size_t size, uint16_t* result);
NQ_EXPORT size_t NQLeb128DecodeUint32(const void* data, size_t size, uint32_t* result);
NQ_EXPORT size_t NQLeb128DecodeUint64(const void* data, size_t size, uint64_t* result);

typedef struct NQLeb128Dec NQLeb128Dec;
struct NQLeb128Dec {
  uint64_t valueUint64;
  unsigned bitWidth;
  bool isSigned;
};

NQ_EXPORT void NQLeb128Dec_init(NQLeb128Dec*, bool isSigned);
NQ_EXPORT bool NQLeb128Dec_update(NQLeb128Dec*, uint8_t byte);

static inline unsigned NQLeb128Dec_bitWidth(const NQLeb128Dec* thiz)
{
  return thiz->bitWidth;
}

static inline unsigned NQLeb128Dec_size(const NQLeb128Dec* thiz)
{
  return (thiz->bitWidth + 7) / 8;
}

static inline int8_t NQLeb128Dec_valueInt8(const NQLeb128Dec* thiz)
{
  return (int8_t)thiz->valueUint64;
}

static inline int16_t NQLeb128Dec_valueInt16(const NQLeb128Dec* thiz)
{
  return (int16_t)thiz->valueUint64;
}

static inline int32_t NQLeb128Dec_valueInt32(const NQLeb128Dec* thiz)
{
  return (int32_t)thiz->valueUint64;
}

static inline int64_t NQLeb128Dec_valueInt64(const NQLeb128Dec* thiz)
{
  return (int64_t)thiz->valueUint64;
}

static inline uint8_t NQLeb128Dec_valueUint8(const NQLeb128Dec* thiz)
{
  return (uint8_t)thiz->valueUint64;
}

static inline uint16_t NQLeb128Dec_valueUint16(const NQLeb128Dec* thiz)
{
  return (uint16_t)thiz->valueUint64;
}

static inline uint32_t NQLeb128Dec_valueUint32(const NQLeb128Dec* thiz)
{
  return (uint32_t)thiz->valueUint64;
}

static inline uint64_t NQLeb128Dec_valueUint64(const NQLeb128Dec* thiz)
{
  return thiz->valueUint64;
}

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_LEB128_H */
