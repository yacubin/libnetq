/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_BASE58_H
#define _LIBNETQ_BASE58_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

NQ_EXPORT int NQBase58Encode(const void* inData, size_t inSize, char* outData, size_t outSize);
NQ_EXPORT int NQBase58Decode(const char* inData, size_t inSize, void* outData, size_t outSize);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_BASE58_H */
