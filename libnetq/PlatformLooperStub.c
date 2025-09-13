/*
 * MIT License
 *
 * Copyright (c) 2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/PlatformLooper.h"

#ifndef NQ_OS_WINDOWS

void NQPlatformLooper_init(NQPlatformLooper* thiz)
{
  NQ_UNUSED_PARAM(thiz);
}

void NQPlatformLooper_finilize(NQPlatformLooper* thiz)
{
  NQ_UNUSED_PARAM(thiz);
}

void NQPlatformLooper_wake(NQPlatformLooper* thiz)
{
  NQ_UNUSED_PARAM(thiz);
}

void NQPlatformLooper_stop(NQPlatformLooper* thiz)
{
  NQ_UNUSED_PARAM(thiz);
}

int NQPlatformLooper_poll(NQPlatformLooper* thiz, int64_t timeout)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(timeout);

  return NQ_LOOPER_POLL_ERROR;
}

bool NQPlatformLooper_getMessage(NQPlatformLooper* thiz, NQMessage* message)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(message);

  return false;
}

#endif
