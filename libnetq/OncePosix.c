/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/Once.h"

#ifdef NQ_OS_UNIX

int NQOnce_call(NQOnce* once, NQOnceCallback callback)
{
  return pthread_once(once, callback);
}

#endif /* NQ_OS_UNIX */
