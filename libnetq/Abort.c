/*
 * MIT License
 *
 * Copyright (c) 2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/Abort.h"

#ifdef NQ_COMPILER_MSVC
# include <intrin.h>
#endif

#ifdef NQ_SYS_LINUX
# include <linux/bug.h>
#endif

void NQAbort(void)
{
#if NQ_HAS_BUILTIN(__builtin_trap)
  __builtin_trap();
#elif defined(NQ_COMPILER_MSVC)
  __debugbreak();
#elif defined(NQ_SYS_LINUX)
  BUG();
#endif
  for (;;) {
    *((int*)~((uintptr_t)0)) = *((int*)0) = 0xDEADBEEF;
    ((int(*)(void))~((uintptr_t)0))();
    ((int(*)(void))0)();
  }
}
