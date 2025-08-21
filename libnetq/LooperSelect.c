/*
 * MIT License
 *
 * Copyright (c) 2021-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/Looper.h"

#ifdef NQ_OS_UNIX
#include <sys/select.h>
#endif

#include <string.h>

#include <libnetq/Math.h>
#include <libnetq/Malloc.h>
#include <libnetq/EventWakeup.h>
#include <libnetq/TimeVal.h>
#include <libnetq/Assert.h>

struct Source {
  NQLooperSource* addr;
  NQLooperSource data;
};

struct Looper {
  NQEventWakeup wakeup;
  NQMutex mutex;
  size_t sourcesCount;
  struct Source sources[FD_SETSIZE];

  bool hasWakeup;
  size_t performSourcesCount;
  NQLooperSource performSources[FD_SETSIZE];
};

static int Looper_init(NQLooper* looper)
{
  struct Looper* thiz = (struct Looper*)NQMalloc(sizeof(struct Looper));
  looper->priv = thiz;

  NQMutex_init(&thiz->mutex);
  NQEventWakeup_init(&thiz->wakeup);

  thiz->sourcesCount = 0;
  memset(&thiz->sources, 0, sizeof(thiz->sources));

  thiz->hasWakeup = false;

  thiz->performSourcesCount = 0;
  memset(&thiz->performSources, 0, sizeof(thiz->performSources));

  return 0;
}

static void Looper_finalize(NQLooper* looper)
{
  struct Looper* impl = (struct Looper*)looper->priv;

  NQEventWakeup_finalize(&impl->wakeup);
  NQMutex_destroy(&impl->mutex);
  NQFree(looper->priv);
}

static void Looper_wake(NQLooper* looper)
{
  struct Looper* impl = (struct Looper*)looper->priv;
  uint64_t value;
  value = (uintptr_t)&value;
  NQEventWakeup_sendUint64(&impl->wakeup, &value);
}

static int Looper_fillFDSet(struct Looper* thiz, int wakeup, fd_set* fdSet)
{
  size_t index = 0;

  FD_SET(wakeup, fdSet);
  int fdMax = wakeup;

  NQMutex_lock(&thiz->mutex);
  while (index < thiz->sourcesCount) {
    int fd = NQLooperSource_getFileDescriptor(&thiz->sources[index].data);
    FD_SET(fd, fdSet);
    fdMax = NQGetMax(fd, fdMax);
    index++;
  }
  NQMutex_unlock(&thiz->mutex);

  return fdMax;
}

static size_t Looper_getSourcesByFDSet(struct Looper* thiz, fd_set* fdSet, NQLooperSource* result)
{
  size_t n = 0;
  size_t index = 0;

  NQMutex_lock(&thiz->mutex);
  while (index < thiz->sourcesCount) {
    int fd = NQLooperSource_getFileDescriptor(&thiz->sources[index].data);
    if (FD_ISSET(fd, fdSet)) {
      *result++ = thiz->sources[index].data;
      n++;
    }
    index++;
  }
  NQMutex_unlock(&thiz->mutex);

  return n;
}

static int Looper_poll(NQLooper* looper, int64_t timeout)
{
  struct Looper* thiz = (struct Looper*)looper->priv;

  fd_set fdSet;
  struct timeval tv;

  if (thiz->hasWakeup || 0 < thiz->performSourcesCount)
    return NQ_LOOPER_POLL_MESSAGE;

  FD_ZERO(&fdSet);

  int wakeup = NQEventWakeup_handle(&thiz->wakeup);
  int fdMax = Looper_fillFDSet(thiz, wakeup, &fdSet);
  int waitResult = select(fdMax + 1, &fdSet, NULL, NULL, NQTimeToTimeVal(timeout, &tv));

  if (waitResult < 0)
    return NQ_LOOPER_POLL_ERROR;

  if (waitResult == 0)
    return NQ_LOOPER_POLL_TIMEOUT;

  if (FD_ISSET(wakeup, &fdSet)) {
    uint64_t value;
    NQEventWakeup_recvUint64(&thiz->wakeup, &value);
    thiz->hasWakeup = true;
  }

  thiz->performSourcesCount = Looper_getSourcesByFDSet(thiz, &fdSet, thiz->performSources);

  return NQ_LOOPER_POLL_MESSAGE;
}

static bool Looper_attachSource(NQLooper* looper, NQLooperSource* source)
{
  struct Looper* thiz = (struct Looper*)looper->priv;
  if (source == NULL || source->type != NQ_SOURCE_FD)
    return false;
  
  bool success = true;

  NQMutex_lock(&thiz->mutex);
  if (NQ_ARRAY_LENGTH(thiz->sources) <= thiz->sourcesCount)
    success = false;
  else {
    struct Source* src = &thiz->sources[thiz->sourcesCount++];
    src->addr = source;
    src->data = *source;
  }
  NQMutex_unlock(&thiz->mutex);

  if (success)
    Looper_wake(looper);

  return success;
}

static bool Looper_detachSource(NQLooper* looper, NQLooperSource* source)
{
  struct Looper* thiz = (struct Looper*)looper->priv;

  if (source == NULL || source->type != NQ_SOURCE_FD)
    return false;

  size_t index = 0;

  bool success = false;
  NQMutex_lock(&thiz->mutex);
  while (index < thiz->sourcesCount) {
    struct Source* src = &thiz->sources[index];
    if (src->addr == source) {
      success = true;
      thiz->sourcesCount--;
      if (thiz->sourcesCount != 0 && thiz->sourcesCount != index)
        *src = thiz->sources[thiz->sourcesCount];
      break;
    }
    index++;
  }
  NQMutex_unlock(&thiz->mutex);

  if (success)
    Looper_wake(looper);

  return success;
}

static void Looper_stop(NQLooper* looper)
{
  looper->stopLoop = true;
}

static bool Looper_getMessage(NQLooper* looper, NQMessage* message)
{
  struct Looper* thiz = (struct Looper*)looper->priv;

  if (thiz->hasWakeup) {
    message->type = NQ_MESSAGE_WAKEUP;
    thiz->hasWakeup = false;
    return true;
  }
  
  if (!thiz->performSourcesCount)
    return false;

  struct NQFDEvent event;
  event.type = NQ_EVENT_FD;

  size_t index = 0;
  while (index < thiz->performSourcesCount) {
    NQLooperSource* src = &thiz->performSources[index];
    int fd = NQLooperSource_getFileDescriptor(src);
    event.fd = fd;
    NQLooperSource_handleEvent(src, (NQEvent*)&event);
    index++;
  }
  thiz->performSourcesCount = 0;

  return false;
}

const struct NQLooperOperations SelectLooperOperations = {
  0,
  &Looper_init,
  &Looper_finalize,
  NULL, // dispatch
  &Looper_wake,
  &Looper_stop,
  &Looper_poll,
  &Looper_attachSource,
  &Looper_detachSource,
  NULL, // startTimer
  NULL, // stopTimer
  &Looper_getMessage,
};
