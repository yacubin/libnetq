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

#include <libnetq/Math.h>

#ifdef NQ_OS_WINDOWS
#include <winsock.h>
#else
#include <sys/time.h>
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

double NQTimeValToSecond(const NQTimeVal* tv)
{
  if (tv == NULL)
    return HUGE_VAL;

  return tv->tv_sec + tv->tv_usec / 1000000.0;
}
