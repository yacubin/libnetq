/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_SYNC_STUB_ONCE_H
#define _LIBNETQ_SYNC_STUB_ONCE_H

#include <libnetq/ErrorCode.h>
#include <libnetq/sync/OnceCallback.h>

typedef struct NQOnce NQOnce;

struct NQOnce {
  int dummy;
};

#define NQ_ONCE_INIT { 0 }

static inline int NQOnce_call(NQOnce* once, NQOnceCallback callback)
{
  NQ_UNUSED_PARAM(once);
  NQ_UNUSED_PARAM(callback);
  return NQ_ENOTSUP;
}

#endif /* _LIBNETQ_SYNC_STUB_ONCE_H */
