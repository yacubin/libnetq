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

#ifdef __has_builtin
# if __has_builtin(__builtin_trap)
#  define USE_BUILTIN_TRAP
# endif
#endif

#ifdef NQ_COMPILER_MSVC
# include <intrin.h>
# define HAVE___DEBUGBREAK
#endif

void NQAbort()
{
#if defined(USE_BUILTIN_TRAP)
  __builtin_trap();
#elif defined(HAVE___DEBUGBREAK)
  __debugbreak();
#endif

  *((int*)~((uintptr_t)0)) = *((int*)0) = 0xDEADBEEF;

  ((int(*)(void))~((uintptr_t)0))();
  ((int(*)(void))0)();
}
