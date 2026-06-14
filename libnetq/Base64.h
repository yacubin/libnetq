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

#define NQ_BASE64_NONPAD (1 << 2)
#define NQ_BASE64_URL    (1 << 3)

NQ_EXPORT int NQBase64EncodeEx(const void* inData, size_t inSize, char* outData, size_t outSize, int flags);
NQ_EXPORT int NQBase64DecodeEx(const char* inData, size_t inSize, void* outData, size_t outSize, int flags);

NQ_EXPORT int NQBase64Encode(const void* inData, size_t inSize, char* outData, size_t outSize);
NQ_EXPORT int NQBase64Decode(const char* inData, size_t inSize, void* outData, size_t outSize);
NQ_EXPORT int NQBase64URLEncode(const void* inData, size_t inSize, char* outData, size_t outSize);
NQ_EXPORT int NQBase64URLDecode(const char* inData, size_t inSize, void* outData, size_t outSize);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_BASE64_H */
