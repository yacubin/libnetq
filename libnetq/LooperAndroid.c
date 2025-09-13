/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/Looper.h"

#ifdef NQ_OS_ANDROID

#include <android/looper.h>
#include <android/input.h>

#include <libnetq/CStrBase.h>
#include <libnetq/Malloc.h>
#include <libnetq/Limits.h>
#include <libnetq/Assert.h>
#include <libnetq/Log.h>
#include <libnetq/LooperSource.h>

struct Source {
  NQLooperSource* addr;
  NQLooperSource data;
};

struct Looper {
  ALooper* nativeLooper;
  NQMutex mutex;
  size_t sourcesCount;
  struct Source sources[64];

  bool hasWakeup;
  bool hasPerformSource;
  NQLooperSource performSource;
};

static int Looper_init(NQLooper* looper)
{
  ALooper* nativeLooper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
  if (nativeLooper == NULL)
    return -1;

  struct Looper* thiz = (struct Looper*)NQMalloc(sizeof(struct Looper));
  looper->priv = thiz;

  NQMutex_init(&thiz->mutex);
  thiz->nativeLooper = nativeLooper;

  thiz->sourcesCount = 0;
  memset(&thiz->sources, 0, sizeof(thiz->sources));

  thiz->hasWakeup = false;
  thiz->hasPerformSource = false;
  memset(&thiz->performSource, 0, sizeof(thiz->performSource));

  return 0;
}

static void Looper_finalize(NQLooper* looper)
{
  struct Looper* thiz = (struct Looper*)looper->priv;
  NQMutex_destroy(&thiz->mutex);
  NQFree(looper->priv);
}

static void Looper_wake(NQLooper* looper)
{
  struct Looper* thiz = (struct Looper*)looper->priv;
  ALooper_wake(thiz->nativeLooper);
}

static int Looper_poll(NQLooper* looper, int64_t timeout)
{
  struct Looper* thiz = (struct Looper*)looper->priv;

  if (thiz->hasWakeup || thiz->hasPerformSource)
    return NQ_LOOPER_POLL_MESSAGE;
  
  int events;
  int waitResult = ALooper_pollOnce((int)timeout, NULL, &events, NULL);

  if (waitResult == ALOOPER_POLL_ERROR)
    return NQ_LOOPER_POLL_ERROR;

  if (waitResult == ALOOPER_POLL_TIMEOUT)
    return NQ_LOOPER_POLL_TIMEOUT;

  /*
    ALOOPER_POLL_CALLBACK
  */

  if (waitResult == ALOOPER_POLL_WAKE)
    thiz->hasWakeup = true;
  else if (waitResult >= 0) {
    thiz->hasPerformSource = true;
    NQMutex_lock(&thiz->mutex);
    if (NQ_ARRAY_LENGTH(thiz->sources) <= waitResult)
      thiz->hasPerformSource = false; // NQ_LOGE
    else
      thiz->performSource = thiz->sources[waitResult].data;
    NQMutex_unlock(&thiz->mutex);
  }
  
  return NQ_LOOPER_POLL_MESSAGE;
}

static bool Looper_attachSource(NQLooper* looper, NQLooperSource* source)
{
  struct Looper* thiz = (struct Looper*)looper->priv;
  if (source == NULL || source->type != NQ_SOURCE_AINPUT)
    return false;

  int32_t ident = -1;
  AInputQueue* inputQueue = NQLooperSource_getInputQueue(source);

  NQMutex_lock(&thiz->mutex);
  if (thiz->sourcesCount < NQ_ARRAY_LENGTH(thiz->sources)) {
    ident = thiz->sourcesCount++;
    struct Source* src = &thiz->sources[ident];
    src->addr = source;
    src->data = *source;
  }
  NQMutex_unlock(&thiz->mutex);

  if (ident < 0)
    return false;

  AInputQueue_attachLooper(inputQueue, thiz->nativeLooper, ident, NULL, NULL);
  return true;
}

static bool Looper_detachSource(NQLooper* looper, NQLooperSource* source)
{
  struct Looper* thiz = (struct Looper*)looper->priv;
  if (source == NULL || source->type != NQ_SOURCE_AINPUT)
    return false;

  size_t index = 0;
  AInputQueue* inputQueue = NULL;
  NQMutex_lock(&thiz->mutex);
  while (index < thiz->sourcesCount) {
    struct Source* src = &thiz->sources[index];
    if (src->addr == source) {
      inputQueue = NQLooperSource_getInputQueue(&src->data);
      thiz->sourcesCount--;
      if (thiz->sourcesCount != 0 && thiz->sourcesCount != index)
        *src = thiz->sources[thiz->sourcesCount];
      break;
    }
    index++;
  }
  NQMutex_unlock(&thiz->mutex);

  if (inputQueue == NULL)
    return false;

  AInputQueue_detachLooper(inputQueue);
  return true;
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

  if (thiz->hasPerformSource) {
    struct NQInputEvent event;
    event.type = NQ_EVENT_AINPUT;
    event.inputQueue = NQLooperSource_getInputQueue(&thiz->performSource);
    for (;;) {
      int32_t available = AInputQueue_getEvent(event.inputQueue, &event.inputEvent);
      if (available < 0)
        break;
      NQLooperSource_handleEvent(&thiz->performSource, (NQEvent*)&event);
    }
    thiz->hasPerformSource = false;
  }

  return false;
}

const struct NQLooperOperations AndroidLooperOperations = {
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

#endif
