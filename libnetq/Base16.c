/*
 * MIT License
 *
 * Copyright (c) 2023-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/Base16.h"

#include <libnetq/CType.h>
#include <libnetq/string/CStrBase.h>

static const char lowerDigits[17] = "0123456789abcdef";
static const char upperDigits[17] = "0123456789ABCDEF";

int NQBase16Encode(const uint8_t* inStart, const uint8_t* inEnd, char* outStart, char* outEnd, int flags)
{
  if (inStart == NULL)
    return -1;

  if (inEnd == NULL)
    inEnd = inStart + strlen((const char*)inStart);

  int result = (inEnd - inStart) * 2;
  if (outStart == NULL)
    return result;

  const char* hexDigits = (flags & NQ_BASE16_LOWER) ? lowerDigits : upperDigits;

  while (inStart < inEnd) {
    if (outEnd <= outStart) {
      return result;
    }
    uint8_t b = *inStart;
    *outStart++ = hexDigits[(b >> 4) & 0x0f];
    if (outEnd <= outStart) {
      return result;
    }
    *outStart++ = hexDigits[b & 0x0f];
    inStart++;
  }

  if (outStart < outEnd) {
    *outStart = '\0';
  }

  return result;
}

int NQBase16Decode(const char* inStart, const char* inEnd, uint8_t* outStart, uint8_t* outEnd, int flags)
{
  NQ_UNUSED_PARAM(flags);

  if (inStart == NULL)
    return -1;

  if (inEnd == NULL)
    inEnd = inStart + strlen((const char*)inStart);

  int result = (inEnd - inStart);
  if (result & 1)
    return -1;
  result /= 2;

  if (outStart == NULL)
    return result;

  while (inStart < inEnd) {
    char upperCharacter = *inStart++;
    char lowerCharacter = *inStart++;
    if (!NQIsHexDigit(upperCharacter) || !NQIsHexDigit(lowerCharacter)) {
      return -1;
    }
    if (outStart && outStart < outEnd) {
      *outStart++ = NQToHexValue(upperCharacter) << 4 | NQToHexValue(lowerCharacter);
    }
    inStart++;
  }

  return result;
}
