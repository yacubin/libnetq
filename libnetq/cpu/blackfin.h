/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CPU_BLACKFIN_H
#define _LIBNETQ_CPU_BLACKFIN_H

#define NQ_FOR_EACH_CPU_BFIN(macro) \
  macro(__bfin)   \
  macro(__bfin__) \
  macro(bfin)     \
  macro(BFIN)

#if  defined (__bfin)   \
  || defined (__bfin__) \
  || defined (bfin)     \
  || defined (BFIN)

#define NQ_CPU_BFIN 1
#define NQ_CPU_NAME "Blackfin"
#define NQ_CPU_ABI "blackfin"
#define NQ_CPU_32BIT 1

#endif

#endif /* _LIBNETQ_CPU_BLACKFIN_H */
