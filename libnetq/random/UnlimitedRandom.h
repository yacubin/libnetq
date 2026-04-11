/*
 * MIT License
 *
 * Copyright (c) 2020-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_RANDOM_UNLIMITEDRANDOM_H
#define _LIBNETQ_RANDOM_UNLIMITEDRANDOM_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

NQ_EXPORT int NQGetUnlimitedRandom(void* data, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_RANDOM_UNLIMITEDRANDOM_H */
