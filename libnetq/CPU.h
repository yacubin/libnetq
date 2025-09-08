/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CPU_H
#define _LIBNETQ_CPU_H

#include <libnetq/Basic.h>

#include <libnetq/cpu/alpha.h>
#include <libnetq/cpu/amd64.h>
#include <libnetq/cpu/arm.h>
#include <libnetq/cpu/arm64.h>
#include <libnetq/cpu/blackfin.h>
#include <libnetq/cpu/convex.h>
#include <libnetq/cpu/epiphany.h>
#include <libnetq/cpu/hppa.h>
#include <libnetq/cpu/ia64.h>
#include <libnetq/cpu/m68k.h>
#include <libnetq/cpu/mips.h>
#include <libnetq/cpu/ppc.h>
#include <libnetq/cpu/pyramid.h>
#include <libnetq/cpu/riscv.h>
#include <libnetq/cpu/rs6k.h>
#include <libnetq/cpu/s370.h>
#include <libnetq/cpu/s390.h>
#include <libnetq/cpu/sh.h>
#include <libnetq/cpu/sparc.h>
#include <libnetq/cpu/tms320.h>
#include <libnetq/cpu/tms470.h>
#include <libnetq/cpu/x86.h>
#include <libnetq/cpu/z.h>

#ifndef NQ_CPU_NAME
# define NQ_CPU_NAME "Unknown"
#endif

#ifndef NQ_CPU_ABI
# define NQ_CPU_ABI "unknown"
#endif

#if !defined(NQ_CPU_LITTLE_ENDIAN) && !defined(NQ_CPU_BIG_ENDIAN)
# define NQ_CPU_LITTLE_ENDIAN 1
#endif

#if !defined(NQ_CPU_32BIT) && !defined(NQ_CPU_64BIT)
# ifdef __LP64__
#  define NQ_CPU_64BIT 1
# else
#  define NQ_CPU_32BIT 1
# endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

NQ_EXPORT unsigned NQGetNumberOfProcessors();

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_CPU_H */
