/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_BASE64_H
#define _LIBNETQ_BASE64_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NQ_BASE64_CR     (1 << 0)
#define NQ_BASE64_LF     (1 << 1)
#define NQ_BASE64_CRLF   (NQ_BASE64_CR | NQ_BASE64_LF)
#define NQ_BASE64_NONPAD (1 << 2)
#define NQ_BASE64_URL    (1 << 3)

NQ_EXPORT int NQBase64Encode(const uint8_t* inStart, const uint8_t* inEnd, char* outStart, char* outEnd, int flags);
NQ_EXPORT int NQBase64Decode(const char* inStart, const char* inEnd, uint8_t* outStart, uint8_t* outEnd, int flags);
NQ_EXPORT int NQBase64URLEncode(const uint8_t* inStart, const uint8_t* inEnd, char* outStart, char* outEnd);
NQ_EXPORT int NQBase64URLDecode(const char* inStart, const char* inEnd, uint8_t* outStart, uint8_t* outEnd);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_BASE64_H */
