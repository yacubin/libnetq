/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CPU_ARM64_H
#define _LIBNETQ_CPU_ARM64_H

#define NQ_FOR_EACH_CPU_ARM64(macro) \
  macro(__aarch64__) \
  macro(__arm64__)

#if  defined (__aarch64__) \
  || defined (__arm64__)

#define NQ_CPU_ARM64 1
#define NQ_CPU_NAME "ARM64"
#define NQ_CPU_ABI "arm64-v8a"
#define NQ_CPU_64BIT 1

#endif

#endif /* _LIBNETQ_CPU_ARM64_H */
