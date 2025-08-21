/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CPU_TMS470_H
#define _LIBNETQ_CPU_TMS470_H

#define NQ_FOR_EACH_CPU_TMS470(macro) \
  macro(__TMS470__)

#if defined (__TMS470__)

#define NQ_CPU_TMS470 1
#define NQ_CPU_NAME "TMS470"
#define NQ_CPU_ABI "tms470"

#endif

#endif /* _LIBNETQ_CPU_TMS470_H */
