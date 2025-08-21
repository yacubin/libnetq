/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CPU_ALPHA_H
#define _LIBNETQ_CPU_ALPHA_H

#define NQ_FOR_EACH_CPU_ALPHA(macro) \
  macro(__alpha__)     \
  macro(__alpha)       \
  macro(_M_ALPHA)      \
  macro(__alpha_ev4__) \
  macro(__alpha_ev5__) \
  macro(__alpha_ev6__)

#if  defined (__alpha__)     \
  || defined (__alpha)       \
  || defined (_M_ALPHA)      \
  || defined (__alpha_ev4__) \
  || defined (__alpha_ev5__) \
  || defined (__alpha_ev6__)

#define NQ_CPU_ALPHA 1
#define NQ_CPU_NAME "DEC Alpha"
#define NQ_CPU_ABI "alpha"
#define NQ_CPU_LITTLE_ENDIAN 1
#define NQ_CPU_64BIT 1

#endif

#endif /* _LIBNETQ_CPU_ALPHA_H */
