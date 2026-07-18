/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CPU_WASM_H
#define _LIBNETQ_CPU_WASM_H

#define NQ_FOR_EACH_CPU_WASM(macro) \
  macro(__wasm) \
  macro(__wasm__) \
  macro(__wasm32) \
  macro(__wasm32__) \
  macro(__wasm64) \
  macro(__wasm64__)

#if  defined (__wasm) \
  || defined (__wasm__) \
  || defined (__wasm32) \
  || defined (__wasm32__) \
  || defined (__wasm64) \
  || defined (__wasm64__)

#define NQ_CPU_WASM 1
#define NQ_CPU_NAME "WebAssembly"
#define NQ_CPU_LITTLE_ENDIAN 1

#if defined(__wasm32) || defined(__wasm32__)
# define NQ_CPU_ABI "wasm32"
# define NQ_CPU_32BIT 1
#endif

#if defined(__wasm64) || defined(__wasm64__) || defined(__LP64__)
# define NQ_CPU_ABI "wasm64"
# define NQ_CPU_64BIT 1
#endif

#endif

#endif /* _LIBNETQ_CPU_WASM_H */
