/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_ENV_H
#define _LIBNETQ_ENV_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

NQ_EXPORT int NQEnvGet(const char* name, char* value, size_t n);
NQ_EXPORT int NQEnvSet(const char* name, const char* value);
NQ_EXPORT int NQEnvUnset(const char* name);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_ENV_H */
