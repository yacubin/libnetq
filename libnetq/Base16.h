/*
 * MIT License
 *
 * Copyright (c) 2023-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_BASE16_H
#define _LIBNETQ_BASE16_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NQ_BASE16_UPPER (0)
#define NQ_BASE16_LOWER (1)

NQ_EXPORT int NQBase16Encode(const uint8_t* inStart, const uint8_t* inEnd, char* outStart, char* outEnd, int flags);
NQ_EXPORT int NQBase16Decode(const char* inStart, const char* inEnd, uint8_t* outStart, uint8_t* outEnd, int flags);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_BASE16_H */
