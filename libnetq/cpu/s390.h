/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CPU_S390_H
#define _LIBNETQ_CPU_S390_H

#define NQ_FOR_EACH_CPU_S390(macro) \
  macro(__s390__) \
  macro(__s390x__)

#if  defined (__s390__) \
  || defined (__s390x__)

#define NQ_CPU_S390 1
#define NQ_CPU_NAME "System/390"

#if defined(__s390x__)
#define NQ_CPU_ABI "s390x"
#define NQ_CPU_64BIT 1
#else
#define NQ_CPU_ABI "s390"
#define NQ_CPU_32BIT 1
#endif

#endif

#endif /* _LIBNETQ_CPU_S390_H */
