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

#ifdef NQ_OS_DARWIN

#include <libnetq/Assert.h>

void NQPlatformLooper_init(NQPlatformLooper* thiz)
{
  thiz->runLoop = CFRunLoopGetCurrent();
  thiz->lastResult = 0;
  CFRetain(thiz->runLoop);
}

void NQPlatformLooper_finilize(NQPlatformLooper* thiz)
{
  CFRelease(thiz->runLoop);
}

void NQPlatformLooper_wake(NQPlatformLooper* thiz)
{
  CFRunLoopWakeUp(thiz->runLoop);
}

void NQPlatformLooper_stop(NQPlatformLooper* thiz)
{
  CFRunLoopStop(thiz->runLoop);
}

int NQPlatformLooper_poll(NQPlatformLooper* thiz, int64_t timeout)
{
  CFRunLoopRunResult waitResult = CFRunLoopRunInMode(kCFRunLoopDefaultMode, timeout / NQ_MSECS_PER_SEC, true);

  if (waitResult == kCFRunLoopRunTimedOut)
    return NQ_LOOPER_POLL_TIMEOUT;

  if (waitResult == kCFRunLoopRunStopped || waitResult == kCFRunLoopRunFinished) {
    thiz->lastResult = waitResult;
    return NQ_LOOPER_POLL_MESSAGE;
  }
  else if (waitResult == kCFRunLoopRunHandledSource) {
    thiz->lastResult = waitResult;
    return NQ_LOOPER_POLL_SOURCE;
  }

  NQ_ASSERT_NOT_REACHED();
  return NQ_LOOPER_POLL_ERROR;
}

bool NQPlatformLooper_getMessage(NQPlatformLooper* thiz, NQMessage* message)
{
  switch (thiz->lastResult) {
  case kCFRunLoopRunStopped:
  case kCFRunLoopRunFinished:
    message->type = NQ_MESSAGE_QUIT;
    thiz->lastResult = 0;
    return true;
  case kCFRunLoopRunHandledSource:
    message->type = NQ_MESSAGE_NORMAL;
    thiz->lastResult = 0;
    return true;
  case kCFRunLoopRunTimedOut:
    return false;
  };

  return false;
}

#endif
