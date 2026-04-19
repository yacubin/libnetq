/*
 * MIT License
 *
 * Copyright (c) 2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/Once.h"

#ifdef NQ_SYS_LINUX

int NQOnce_call(NQOnce* once, NQOnceCallback callback)
{
  if (atomic_cmpxchg(&once->flag, 0, 1) == 0)
    callback();
  return 0;
}

#endif /* NQ_SYS_LINUX */
