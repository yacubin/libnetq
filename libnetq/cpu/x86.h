/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CPU_X86_H
#define _LIBNETQ_CPU_X86_H

#define NQ_FOR_EACH_CPU_X86(macro) \
  macro(i386)          \
  macro(__i386)        \
  macro(__i386__)      \
  macro(__i486__)      \
  macro(__i586__)      \
  macro(__i686__)      \
  macro(__IA32__)      \
  macro(_M_I86)        \
  macro(_M_IX86)       \
  macro(__X86__)       \
  macro(_X86_)         \
  macro(__THW_INTEL__) \
  macro(__I86__)       \
  macro(__INTEL__)     \
  macro(__386)

#if  defined(i386)          \
  || defined(__i386)        \
  || defined(__i386__)      \
  || defined(__i486__)      \
  || defined(__i586__)      \
  || defined(__i686__)      \
  || defined(__IA32__)      \
  || defined(_M_I86)        \
  || defined(_M_IX86)       \
  || defined(__X86__)       \
  || defined(_X86_)         \
  || defined(__THW_INTEL__) \
  || defined(__I86__)       \
  || defined(__INTEL__)     \
  || defined(__386)

#define NQ_CPU_X86 1
#define NQ_CPU_NAME "Intel x86"
#define NQ_CPU_ABI "x86"
#define NQ_CPU_LITTLE_ENDIAN 1
#define NQ_CPU_32BIT 1

#endif

#endif /* _LIBNETQ_CPU_X86_H */
