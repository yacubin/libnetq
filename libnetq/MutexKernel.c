/*
 * MIT License
 *
 * Copyright (c) 2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/Mutex.h"

#ifdef NQ_SYS_LINUX

void NQMutex_init(NQMutex* mutex)
{
  mutex_init(mutex);
}

void NQMutex_destroy(NQMutex* mutex)
{
  mutex_destroy(mutex);
}

void NQMutex_lock(NQMutex* mutex)
{
  mutex_lock(mutex);
}

bool NQMutex_trylock(NQMutex* mutex)
{
  return mutex_trylock(mutex) ? true : false;
}

void NQMutex_unlock(NQMutex* mutex)
{
  mutex_unlock(mutex);
}

#endif /* NQ_SYS_LINUX */
