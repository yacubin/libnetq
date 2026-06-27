/*
 * MIT License
 *
 * Copyright (c) 2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CPU_RISCV_H
#define _LIBNETQ_CPU_RISCV_H

#define NQ_FOR_EACH_CPU_RISCV(macro) \
  macro(__riscv)        \
  macro(__riscv_xlen)   \
  macro(__riscv64)      \
  macro(__riscv_atomic) \
  macro(__riscv_mul)    \
  macro(__riscv_muldiv)  \
  macro(__riscv_flen)

#if  defined (__riscv)        \
  || defined (__riscv_xlen)   \
  || defined (__riscv64)      \
  || defined (__riscv_atomic) \
  || defined (__riscv_mul)    \
  || defined (__riscv_muldiv) \
  || defined (__riscv_flen)

#define NQ_CPU_RISCV 1
#define NQ_CPU_NAME "RISC-V"
#define NQ_CPU_ABI "riscv"
#define NQ_CPU_LITTLE_ENDIAN 1

#if defined(__riscv_xlen)
# if __riscv_xlen == 64
#  define NQ_CPU_64BIT 1
# elif __riscv_xlen == 32
#  define NQ_CPU_32BIT 1
# else
#  error Not supported RISC-V bit size
# endif
#elif defined(__riscv64) || defined(__LP64__)
# define NQ_CPU_64BIT 1
#else
# define NQ_CPU_32BIT 1
#endif

#endif

#endif /* _LIBNETQ_CPU_RISCV_H */
