/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CPU_Z_H
#define _LIBNETQ_CPU_Z_H

#define NQ_FOR_EACH_CPU_ZARCH(macro) \
  macro(__zarch__) \
  macro(__SYSC_ZARCH__)

#if  defined (__zarch__) \
  || defined (__SYSC_ZARCH__)

#define NQ_CPU_ZARCH 1
#define NQ_CPU_NAME "z/Architecture"
#define NQ_CPU_ABI "zarch"

#endif

#endif /* _LIBNETQ_CPU_Z_H */
