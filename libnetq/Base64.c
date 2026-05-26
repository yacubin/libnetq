/*
 * MIT License
 *
 * Copyright (c) 2020-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/Base64.h"

#include <libnetq/string/CStrBase.h>
#include <libnetq/Assert.h>

#define UNKN 0xFF

static const char base64EncMap[64] = {
  0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
  0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50,
  0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
  0x59, 0x5A, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66,
  0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E,
  0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76,
  0x77, 0x78, 0x79, 0x7A, 0x30, 0x31, 0x32, 0x33,
  0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x2B, 0x2F
};

static const uint8_t base64DecMap[128] = {
  UNKN, UNKN, UNKN, UNKN, UNKN, UNKN, UNKN, UNKN,
  UNKN, UNKN, UNKN, UNKN, UNKN, UNKN, UNKN, UNKN,
  UNKN, UNKN, UNKN, UNKN, UNKN, UNKN, UNKN, UNKN,
  UNKN, UNKN, UNKN, UNKN, UNKN, UNKN, UNKN, UNKN,
  UNKN, UNKN, UNKN, UNKN, UNKN, UNKN, UNKN, UNKN,
  UNKN, UNKN, UNKN, 0x3E, UNKN, UNKN, UNKN, 0x3F,
  0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B,
  0x3C, 0x3D, UNKN, UNKN, UNKN, UNKN, UNKN, UNKN,
  UNKN, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
  0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
  0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
  0x17, 0x18, 0x19, UNKN, UNKN, UNKN, UNKN, UNKN,
  UNKN, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20,
  0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
  0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30,
  0x31, 0x32, 0x33, UNKN, UNKN, UNKN, UNKN, UNKN
};

static const char base64URLEncMap[64] = {
  0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
  0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50,
  0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
  0x59, 0x5A, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66,
  0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E,
  0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76,
  0x77, 0x78, 0x79, 0x7A, 0x30, 0x31, 0x32, 0x33,
  0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x2D, 0x5F
};

static const uint8_t base64URLDecMap[128] = {
  UNKN, UNKN, UNKN, UNKN, UNKN, UNKN, UNKN, UNKN,
  UNKN, UNKN, UNKN, UNKN, UNKN, UNKN, UNKN, UNKN,
  UNKN, UNKN, UNKN, UNKN, UNKN, UNKN, UNKN, UNKN,
  UNKN, UNKN, UNKN, UNKN, UNKN, UNKN, UNKN, UNKN,
  UNKN, UNKN, UNKN, UNKN, UNKN, UNKN, UNKN, UNKN,
  UNKN, UNKN, UNKN, UNKN, UNKN, 0x3E, UNKN, UNKN,
  0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B,
  0x3C, 0x3D, UNKN, UNKN, UNKN, UNKN, UNKN, UNKN,
  UNKN, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
  0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
  0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
  0x17, 0x18, 0x19, UNKN, UNKN, UNKN, UNKN, 0x3F,
  UNKN, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20,
  0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
  0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30,
  0x31, 0x32, 0x33, UNKN, UNKN, UNKN, UNKN, UNKN
};

int NQBase64Encode(const uint8_t* inStart, const uint8_t* inEnd, char* outStart, char* outEnd, int flags)
{
  uint8_t b;
  const char* map;

  if (inStart == NULL)
    return -1;

  if (inEnd == NULL)
    inEnd = inStart + strlen((const char*)inStart);
  
  size_t size = inEnd - inStart;
  int result = (((int)size + 2) / 3) * 4;
  if (result < size)
    return -1;

  if (flags & NQ_BASE64_NONPAD) {
    size_t rem = size % 3;
    if (rem != 0)
      result -= (3 - rem);
  }

  if (outStart == NULL)
    return result;

  if (outEnd == NULL)
    outEnd = outStart + result + 1;

  if (result == 0) {
    if (outStart < outEnd)
      *outStart = '\0';
    return result;
  }

  if (outEnd <= outStart)
    return -1;

  if (flags & NQ_BASE64_URL)
    map = base64URLEncMap;
  else
    map = base64EncMap;

  for (;;) {
    b = (*inStart >> 2) & 0x3F;
    *outStart++ = map[b];
    if (outEnd <= outStart)
      break;

    b = (*inStart++ << 4) & 0x3F;
    if (inEnd <= inStart) {
      *outStart++ = map[b];
      if (outEnd <= outStart)
        break;
      if (!(flags & NQ_BASE64_NONPAD)) {
        *outStart++ = '=';
        if (outEnd <= outStart)
          break;
        *outStart++ = '=';
        if (outEnd <= outStart)
          break;
      }
      *outStart++ = '\0';
      break;
    }

    b |= (*inStart >> 4) & 0x0F;
    *outStart++ = map[b];
    if (outEnd <= outStart)
      break;

    b = (*inStart++ << 2) & 0x3F;
    if (inEnd <= inStart) {
      *outStart++ = map[b];
      if (outEnd <= outStart)
        break;
      if (!(flags & NQ_BASE64_NONPAD)) {
        *outStart++ = '=';
        if (outEnd <= outStart)
          break;
      }
      *outStart++ = '\0';
      break;
    }

    b |= (*inStart >> 6) & 0x03;
    *outStart++ = map[b];
    if (outEnd <= outStart)
      break;

    b = *inStart++ & 0x3F;
    *outStart++ = map[b];
    if (outEnd <= outStart)
      break;

    if (inEnd <= inStart) {
      if (outStart < outEnd)
        *outStart = '\0';
      break;
    }
  }

  return result;
}

int NQBase64Decode(const char* inStart, const char* inEnd, uint8_t* outStart, uint8_t* outEnd, int flags)
{
  uint8_t curr;
  uint8_t b1, b2, b3, b4;
  int result;
  const uint8_t* map;

  if (inStart == NULL)
    return -1;

  if (inEnd == NULL)
    inEnd = inStart + strlen((const char*)inStart);

  if (inEnd <= inStart) {
    if (outStart < outEnd)
      *outStart = '\0';
    return 0;
  }

  if (outStart == NULL)
    outEnd = NULL;
  else if (outEnd == NULL)
    outEnd = outStart + (inEnd - inStart + 3) / 4 * 3 + 1;

  if (flags & NQ_BASE64_URL)
    map = base64URLDecMap;
  else
    map = base64DecMap;

  result = 0;
  for (;;) {
    curr = (uint8_t)(*inStart++);
    b1 = map[curr];
    if (b1 == UNKN) {
      result = -1;
      break;
    }

    if (inEnd <= inStart) {
      result++;
      if (outStart < outEnd)
        *outStart++ = (b1 << 2);
      break;
    }

    curr = (uint8_t)(*inStart++);
    b2 = map[curr];
    if (b2 == UNKN) {
      result = -1;
      break;
    }

    result++;
    if (outStart < outEnd)
      *outStart++ = (b1 << 2) | (b2 >> 4);

    if (inEnd <= inStart) {
      result++;
      if (outStart < outEnd)
        *outStart++ = ((b2 << 4) & 0xF0);
      break;
    }

    curr = (uint8_t)(*inStart++);
    b3 = map[curr];
    if (b3 == UNKN) {
      if (curr == '=' && inStart < inEnd && *inStart == '=') {
        if (++inStart == inEnd)
          break;
      }
      result = -1;
      break;
    }

    result++;
    if (outStart < outEnd)
      *outStart++ = ((b2 << 4) & 0xF0) | (b3 >> 2);

    if (inEnd <= inStart) {
      result++;
      if (outStart < outEnd)
        *outStart++ = (b3 << 6);
      break;
    }

    curr = (uint8_t)(*inStart++);
    b4 = map[curr];
    if (b4 == UNKN) {
      if (curr == '=' && inStart == inEnd)
        break;
      result = -1;
      break;
    }

    result++;
    if (outStart < outEnd)
      *outStart++ = ((b3 << 6) | b4);

    if (inEnd <= inStart)
      break;
  }

  if (outStart < outEnd)
    *outStart = '\0';

  return result;
}

int NQBase64URLEncode(const uint8_t* inStart, const uint8_t* inEnd, char* outStart, char* outEnd)
{
  return NQBase64Encode(inStart, inEnd, outStart, outEnd, NQ_BASE64_URL);
}

int NQBase64URLDecode(const char* inStart, const char* inEnd, uint8_t* outStart, uint8_t* outEnd)
{
  return NQBase64Decode(inStart, inEnd, outStart, outEnd, NQ_BASE64_URL);
}
