/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CPU_EPIPHANY_H
#define _LIBNETQ_CPU_EPIPHANY_H

#define NQ_FOR_EACH_CPU_EPIPHANY(macro) \
  macro(__epiphany__)

#if defined (__epiphany__)

#define NQ_CPU_EPIPHANY 1
#define NQ_CPU_NAME "Epiphany"
#define NQ_CPU_ABI "epiphany"
#define NQ_CPU_32BIT 1

#endif

#endif /* _LIBNETQ_CPU_EPIPHANY_H */
