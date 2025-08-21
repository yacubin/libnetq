/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CPU_CONVEX_H
#define _LIBNETQ_CPU_CONVEX_H

#define NQ_FOR_EACH_CPU_CONVEX(macro) \
  macro(__convex__)     \
  macro(__convex_c1__)  \
  macro(__convex_c2__)  \
  macro(__convex_c32__) \
  macro(__convex_c34__) \
  macro(__convex_c38__)

#if  defined (__convex__)     \
  || defined (__convex_c1__)  \
  || defined (__convex_c2__)  \
  || defined (__convex_c32__) \
  || defined (__convex_c34__) \
  || defined (__convex_c38__)

#define NQ_CPU_CONVEX 1
#define NQ_CPU_NAME "Convex"
#define NQ_CPU_ABI "convex"
#define NQ_CPU_32BIT 1

#endif

#endif /* _LIBNETQ_CPU_CONVEX_H */
