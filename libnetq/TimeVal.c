/*
 * MIT License
 *
 * Copyright (c) 2021-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/TimeVal.h"

#include <libnetq/Limits.h>

#if defined(NQ_OS_WINDOWS)
# include <winsock2.h>
#elif defined(NQ_OS_UNIX)
# include <sys/time.h>
#else
struct timeval {
  long tv_sec;
  long tv_usec;
};
#endif

NQTimeVal* NQTimeToTimeVal(NQTime time, NQTimeVal* tv)
{
  if (time < 0)
    return NULL;

  if (time == 0) {
    tv->tv_sec = 0;
    tv->tv_usec = 0;
  }
  else {
    tv->tv_sec = time / NQ_MSECS_PER_SEC;
    tv->tv_usec = (time - (NQTime)tv->tv_sec * NQ_MSECS_PER_SEC) * 1000;
  }

  return tv;
}

NQTime NQTimeValToTime(const NQTimeVal* tv)
{
  if (tv == NULL)
    return -1;

  return (NQTime)(tv->tv_sec * NQ_MSECS_PER_SEC + tv->tv_usec / NQ_MSECS_PER_SEC);
}

int64_t NQTimeValToMsecs(const NQTimeVal* tv)
{
  if (tv == NULL)
    return NQ_INT64_MAX;

  return (tv->tv_sec * 1000) + (tv->tv_usec / 1000);
}
