/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CPU_TMS320_H
#define _LIBNETQ_CPU_TMS320_H

#define NQ_FOR_EACH_CPU_TMS320(macro) \
  macro(_TMS320C2XX)       \
  macro(__TMS320C2000__)   \
  macro(_TMS320C28X)       \
  macro(_TMS320C5X)        \
  macro(__TMS320C55X__)    \
  macro(_TMS320C6X)        \
  macro(__TMS320C6X__)     \
  macro(_TMS320C6200)      \
  macro(_TMS320C6400)      \
  macro(_TMS320C6400_PLUS) \
  macro(_TMS320C6600)      \
  macro(_TMS320C6700)      \
  macro(_TMS320C6700_PLUS) \
  macro(_TMS320C6740)

#if  defined (_TMS320C2XX)       \
  || defined (__TMS320C2000__)   \
  || defined (_TMS320C28X)       \
  || defined (_TMS320C5X)        \
  || defined (__TMS320C55X__)    \
  || defined (_TMS320C6X)        \
  || defined (__TMS320C6X__)     \
  || defined (_TMS320C6200)      \
  || defined (_TMS320C6400)      \
  || defined (_TMS320C6400_PLUS) \
  || defined (_TMS320C6600)      \
  || defined (_TMS320C6700)      \
  || defined (_TMS320C6700_PLUS) \
  || defined (_TMS320C6740)

#define NQ_CPU_TMS320 1
#define NQ_CPU_NAME "TMS320"
#define NQ_CPU_ABI "tms320"

#endif

#endif /* _LIBNETQ_CPU_TMS320_H */
