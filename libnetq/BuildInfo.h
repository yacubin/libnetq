/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_BUILDINFO_H
#define _LIBNETQ_BUILDINFO_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

NQ_EXPORT const char* NQGetBuildCPU();
NQ_EXPORT const char* NQGetBuildABI();
NQ_EXPORT const char* NQGetBuildDateTime();

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_BUILDINFO_H */
