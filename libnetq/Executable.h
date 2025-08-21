/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_EXECUTABLE_H
#define _LIBNETQ_EXECUTABLE_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

NQ_EXPORT size_t NQGetCommandLine(char* buffer, size_t n);
NQ_EXPORT size_t NQGetCurrentDirectory(char* buffer, size_t n);
NQ_EXPORT size_t NQGetExecutablePath(char* buffer, size_t n);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_EXECUTABLE_H */
