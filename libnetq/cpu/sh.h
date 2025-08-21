/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CPU_SH_H
#define _LIBNETQ_CPU_SH_H

#define NQ_FOR_EACH_CPU_SH(macro) \
  macro(__sh__)  \
  macro(__sh1__) \
  macro(__sh2__) \
  macro(__sh3__) \
  macro(__SH3__) \
  macro(__SH4__) \
  macro(__SH5__)

#if  defined (__sh__)  \
  || defined (__sh1__) \
  || defined (__sh2__) \
  || defined (__sh3__) \
  || defined (__SH3__) \
  || defined (__SH4__) \
  || defined (__SH5__)

#define NQ_CPU_NAME "SuperH"
#define NQ_CPU_ABI "superh"

#endif

#endif /* _LIBNETQ_CPU_SH_H */
