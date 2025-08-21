/*
 * MIT License
 *
 * Copyright (c) 2023-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_GETUSERNAME_H
#define _LIBNETQ_GETUSERNAME_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

NQ_EXPORT size_t NQGetUserName(char* buffer, size_t n);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_GETUSERNAME_H */
