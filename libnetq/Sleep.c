/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/Sleep.h"

#include <libnetq/Assert.h>

#if defined(NQ_SYS_LINUX)
# include <linux/delay.h>
#elif defined(NQ_OS_WINDOWS)
# include <windows.h>
#else
# include <libnetq/Time.h>
# if _POSIX_C_SOURCE >= 199309L
#  include <time.h>
#  define HAVE_NANOSLEEP 1
# else
#  include <unistd.h>
#  define HAVE_USLEEP 1
# endif
#endif

void NQSleep(int64_t ms)
{
  NQ_ASSERT(ms >= 0);
#if defined(NQ_SYS_LINUX)
  msleep_interruptible(ms);
#elif defined(NQ_OS_WINDOWS)
  NQ_ASSERT(ms <= MAXDWORD);
  Sleep((DWORD)ms);
#elif defined(HAVE_NANOSLEEP)
  struct timespec ts;
  ts.tv_sec = ms / NQ_MSECS_PER_SEC;
  ts.tv_nsec = (ms % NQ_MSECS_PER_SEC) * 1000000;
  nanosleep(&ts, NULL);
#elif defined(HAVE_USLEEP)
  usleep(ms * NQ_MSECS_PER_SEC);
#else
  struct timeval tv;
  int rc = select(0, NULL, NULL, NULL, NQTimeMsToTimeVal(ms, &tv));
  NQ_ASSERT_UNUSED(rc, rc == 0);
#endif
}
