/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CPU_AMD64_H
#define _LIBNETQ_CPU_AMD64_H

#define NQ_FOR_EACH_CPU_AMD64(macro) \
  macro(__amd64__)  \
  macro(__amd64)    \
  macro(__x86_64__) \
  macro(__x86_64)   \
  macro(_M_X64)

#if  defined (__amd64__)  \
  || defined (__amd64)    \
  || defined (__x86_64__) \
  || defined (__x86_64)   \
  || defined (_M_X64)

#define NQ_CPU_AMD64 1
#define NQ_CPU_NAME "AMD64"
#define NQ_CPU_ABI "x86_64"
#define NQ_CPU_LITTLE_ENDIAN 1
#define NQ_CPU_64BIT 1

#endif

#endif /* _LIBNETQ_CPU_AMD64_H */
