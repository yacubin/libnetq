/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_HASH_H
#define _LIBNETQ_HASH_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NQ_HASH_SEED_DEFAULT 0x9E3779B9U

typedef uint32_t NQHashCode;
  
NQ_EXPORT uint32_t NQHashCStringWithSeed(uint32_t hash, const char* characters);
NQ_EXPORT uint32_t NQHashStringWithSeed(uint32_t hash, const char* characters, size_t length);

#define NQHashCString(characters) NQHashCStringWithSeed(NQ_HASH_SEED_DEFAULT, (characters))
#define NQHashString(characters, length) NQHashStringWithSeed(NQ_HASH_SEED_DEFAULT, (characters), (length))

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_HASH_H */
