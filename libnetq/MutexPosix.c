/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/Mutex.h"

#ifdef NQ_OS_UNIX

#include <errno.h>
#include <libnetq/Assert.h>

void NQMutex_init(NQMutex* mutex)
{
  int result = pthread_mutex_init(mutex, NULL);
  NQ_ASSERT_UNUSED(result, !result);
}

void NQMutex_destroy(NQMutex* mutex)
{
  int result = pthread_mutex_destroy(mutex);
  NQ_ASSERT_UNUSED(result, !result);
}

void NQMutex_lock(NQMutex* mutex)
{
  int result = pthread_mutex_lock(mutex);
  NQ_ASSERT_UNUSED(result, !result);
}

bool NQMutex_trylock(NQMutex* mutex)
{
  int result = pthread_mutex_trylock(mutex);

  if (result == 0)
    return true;

  if (result == EBUSY)
    return false;

  NQ_ASSERT_NOT_REACHED();
  return false;
}

void NQMutex_unlock(NQMutex* mutex)
{
  int result = pthread_mutex_unlock(mutex);
  NQ_ASSERT_UNUSED(result, !result);
}

#endif /* NQ_OS_UNIX */
