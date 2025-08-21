/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CPU_SPARC_H
#define _LIBNETQ_CPU_SPARC_H

#define NQ_FOR_EACH_CPU_SPARC(macro) \
  macro(__sparc__)    \
  macro(__sparc)      \
  macro(__sparc_v8__) \
  macro(__sparcv8)    \
  macro(__sparc_v9__) \
  macro(__sparcv9)

#if  defined (__sparc__)    \
  || defined (__sparc)      \
  || defined (__sparc_v8__) \
  || defined (__sparcv8)    \
  || defined (__sparc_v9__) \
  || defined (__sparcv9)

#define NQ_CPU_SPARC 1
#define NQ_CPU_NAME "SPARC"
#define NQ_CPU_ABI "sparc"

#endif

#endif /* _LIBNETQ_CPU_SPARC_H */
