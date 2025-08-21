/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CPU_IA64_H
#define _LIBNETQ_CPU_IA64_H

#define NQ_FOR_EACH_CPU_IA64(macro) \
  macro(__ia64)      \
  macro(__ia64__)    \
  macro(_IA64)       \
  macro(__IA64__)    \
  macro(_M_IA64)     \
  macro(__itanium__)

#if  defined (__ia64)      \
  || defined (__ia64__)    \
  || defined (_IA64)       \
  || defined (__IA64__)    \
  || defined (_M_IA64)     \
  || defined (__itanium__)

#define NQ_CPU_IA64 1
#define NQ_CPU_NAME "Intel Itanium (IA-64)"
#define NQ_CPU_ABI "ia64"
#define NQ_CPU_LITTLE_ENDIAN 1
#define NQ_CPU_64BIT 1

#if !defined(__LP64__)
#define NQ_CPU_IA64_32 1
#endif

#endif

#endif /* _LIBNETQ_CPU_IA64_H */
