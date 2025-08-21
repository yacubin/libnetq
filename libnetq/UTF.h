/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_UTF_H
#define _LIBNETQ_UTF_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif


#define NQIsUChar(character) ((character) >= 0)

NQ_EXPORT size_t NQUCharSize8(NQUChar character);
NQ_EXPORT size_t NQUCharSize16(NQUChar character);
NQ_EXPORT NQUChar NQUCharNext8(const uint8_t* start, const uint8_t* end, const uint8_t** charEnd);
NQ_EXPORT NQUChar NQUCharNext16(const uint16_t* start, const uint16_t* end, const uint16_t** charEnd);
NQ_EXPORT size_t NQUCharPush8(uint8_t* start, uint8_t* end, uint8_t** charEnd, NQUChar character);
NQ_EXPORT size_t NQUCharPush16(uint16_t* start, uint16_t* end, uint16_t** charEnd, NQUChar character);

typedef struct NQUnicodeInfo {
  size_t utf8Size;
  size_t utf16Size;
  size_t utf32Size;
  bool isAllASCII;
} NQUnicodeInfo;

NQ_EXPORT bool NQCalculateUTF8Info(const uint8_t* utf8Start, const uint8_t* utf8End, NQUnicodeInfo* info);
NQ_EXPORT bool NQConvertUTF8ToUTF16(const uint8_t* utf8Start, const uint8_t* utf8End, uint16_t* utf16Start, uint16_t* utf16End, NQUnicodeInfo* info);

NQ_EXPORT bool NQCalculateUTF16Info(const uint16_t* utf16Start, const uint16_t* utf16End, NQUnicodeInfo* info);
NQ_EXPORT bool NQConvertUTF16ToUTF8(const uint16_t* utf16Start, const uint16_t* utf16End, uint8_t* utf8Start, uint8_t* utf8End, NQUnicodeInfo* info);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_UTF_H */
