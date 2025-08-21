/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CPU_S370_H
#define _LIBNETQ_CPU_S370_H

#define NQ_FOR_EACH_CPU_S370(macro) \
  macro(__370__) \
  macro(__THW_370__)

#if  defined (__370__) \
  || defined (__THW_370__)

#define NQ_CPU_S370 1
#define NQ_CPU_NAME "System/370"
#define NQ_CPU_ABI "s370"

#endif

#endif /* _LIBNETQ_CPU_S370_H */
