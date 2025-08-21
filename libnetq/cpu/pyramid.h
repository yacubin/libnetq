/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CPU_PYRAMID_H
#define _LIBNETQ_CPU_PYRAMID_H

#define NQ_FOR_EACH_CPU_PYR(macro) \
  macro(pyr)

#if defined (pyr)

#define NQ_CPU_PYR 1
#define NQ_CPU_NAME "Pyramid 9810"
#define NQ_CPU_ABI "pyramid"
#define NQ_CPU_BIG_ENDIAN 1
#define NQ_CPU_32BIT 1

#endif

#endif /* _LIBNETQ_CPU_PYRAMID_H */
