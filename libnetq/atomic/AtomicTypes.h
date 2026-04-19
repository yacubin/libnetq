/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_ATOMIC_ATOMICTYPES_H
#define _LIBNETQ_ATOMIC_ATOMICTYPES_H

#include <libnetq/Basic.h>

typedef struct NQAtomic32 NQAtomic32; // NQAtomic
typedef struct NQAtomic64 NQAtomic64;

struct NQAtomic32 {
  int32_t counter;
};

struct NQAtomic64 {
  int64_t counter;
};

#endif /* _LIBNETQ_ATOMIC_ATOMICTYPES_H */
