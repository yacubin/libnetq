/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/sync/win32/Once.h"

#ifdef NQ_OS_WINDOWS

static BOOL CALLBACK InitHandleFunction(PINIT_ONCE initOnce, PVOID ptr, PVOID* result)
{
  NQOnceCallback callback = (NQOnceCallback)ptr;
  callback();
  return TRUE;
}

int NQOnce_call(NQOnce* once, NQOnceCallback callback)
{
  if (InitOnceExecuteOnce(once, InitHandleFunction, callback, NULL))
    return 0;
  return (int)GetLastError();
}

#endif /* NQ_OS_WINDOWS */
