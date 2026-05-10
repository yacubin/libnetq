/*
 * MIT License
 *
 * Copyright (c) 2020-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/Looper.h"

#include <libnetq/Cond.h>
#include <libnetq/Time.h>
#include <libnetq/Malloc.h>
#include <libnetq/Once.h>
#include <libnetq/Signal.h>
#include <libnetq/Assert.h>

static NQOnce s_once = NQ_ONCE_INIT;
static volatile bool s_stopLoop = false;

static void NQLooper_performWork(NQLooper* looper);

static void onSignalHandler(int type, void* ptr)
{
  NQ_UNUSED_PARAM(ptr);
  s_stopLoop = true;
}

static void onInitialize(void)
{
  NQSetSignalHandler(kNQSignalTypeSigInt,  onSignalHandler, NULL);
  NQSetSignalHandler(kNQSignalTypeSigTerm, onSignalHandler, NULL);
  NQSetSignalHandler(kNQSignalTypeSigKill, onSignalHandler, NULL);
}

static int64_t NQLooper_timerTimeout(NQLooper* looper)
{
  int64_t timeout;
  NQTimeMs now = NQGetTimeMs();

  NQMutex_lock(&looper->mutex);
  timeout = NQTimerQueue_timeout(looper->timerQueue, now);
  NQMutex_unlock(&looper->mutex);

  return timeout;
}

static void NQLooper_performTimer(NQLooper* looper)
{
  int timeout;
  NQTimerData data;

  for (;;) {
    NQMutex_lock(&looper->mutex);
    timeout = NQTimerQueue_shiftTimer(looper->timerQueue, looper->poolTime, &data);
    NQMutex_unlock(&looper->mutex);

    if (timeout != 0)
      break;

    if (data.handle)
      data.handle(data.userdata);

    if (data.destroy)
      data.destroy(data.userdata);
  }
}

static void NQLooper_poolAll(NQLooper* looper)
{
  int result;
  int64_t timeout;
  NQMessage message;

  while (!looper->stopLoop && !s_stopLoop) {
    timeout = NQLooper_timerTimeout(looper);
    result = looper->ops.poll(looper, timeout);
    looper->poolTime = NQGetTimeMs();

    if (result == NQ_LOOPER_POLL_ERROR)
      break;

    NQLooper_performTimer(looper);

    if (result == NQ_LOOPER_POLL_TIMEOUT)
      continue;

    if (result == NQ_LOOPER_POLL_MESSAGE) {
      while (looper->ops.getMessage(looper, &message)) {
        if (message.type == NQ_MESSAGE_WAKEUP)
          NQLooper_performWork(looper);
        else if (message.type == NQ_MESSAGE_QUIT)
          looper->stopLoop = true;
      }
    }
  }
}

static bool NQLooper_getMessage(NQLooper* looper, NQMessage* message)
{
  NQLooper_performWork(looper);
  return false;
}

void NQLooper_init(NQLooper* looper, const struct NQLooperOperations* ops)
{
  NQOnce_call(&s_once, onInitialize);

  looper->ops = *ops;

  looper->stopLoop = false;
  looper->notified = false;

  NQMutex_init(&looper->mutex);
  looper->tid = NQThreadId();
  looper->timerQueue = NQTimerQueue_create(1000);
  looper->dispatchQueue = NQDispatchQueue_create(0);
  looper->poolTime = NQGetTimeMs();

  if (looper->ops.init)
    looper->ops.init(looper);

  if (looper->ops.dispatch == NULL)
    looper->ops.dispatch = &NQLooper_poolAll;

  if (looper->ops.getMessage == NULL)
    looper->ops.getMessage = &NQLooper_getMessage;
}

void NQLooper_finalize(NQLooper* looper)
{
  if (looper->ops.finalize)
    looper->ops.finalize(looper);

  if (looper->timerQueue) {
    NQTimerData timerData;
    while (NQTimerQueue_shiftTimer(looper->timerQueue, -1, &timerData) >= 0) {
      if (timerData.destroy)
        timerData.destroy(timerData.userdata);
    }
    NQTimerQueue_destroy(looper->timerQueue);
  }

  if (looper->dispatchQueue) {
    NQDispatchData dispatchData;
    while (NQDispatchQueue_shift(looper->dispatchQueue, &dispatchData))
      dispatchData.destroy(dispatchData.userdata);
    NQDispatchQueue_destroy(looper->dispatchQueue);
  }

  NQMutex_finalize(&looper->mutex);
}

bool NQLooper_isLoopThread(const NQLooper* looper)
{
  return looper->tid == NQThreadId();
}

void NQLooper_performWork(NQLooper* looper)
{
  size_t index, size;
  NQDispatchData data;

  NQMutex_lock(&looper->mutex);
  size = NQDispatchQueue_size(looper->dispatchQueue);
  if (size == 0) {
    NQMutex_unlock(&looper->mutex);
    return;
  }
  NQDispatchQueue_shift(looper->dispatchQueue, &data);
  NQMutex_unlock(&looper->mutex);

  if (data.handle)
    data.handle(data.userdata);

  if (data.destroy)
    data.destroy(data.userdata);

  for (index = 1; index < size; ++index) {
    NQMutex_lock(&looper->mutex);
    if (!NQDispatchQueue_shift(looper->dispatchQueue, &data)) {
      NQMutex_unlock(&looper->mutex);
      break;
    }
    NQMutex_unlock(&looper->mutex);

    if (data.handle)
      data.handle(data.userdata);

    if (data.destroy)
      data.destroy(data.userdata);
  }
}

void NQLooper_dispatch(NQLooper* looper)
{
  looper->ops.dispatch(looper);
}

void NQLooper_call(NQLooper* looper, void* userdata, NQDispatchHandleCallback handle, NQDispatchDestroyCallback destroy)
{
  bool success;

  NQDispatchData data;
  data.userdata = userdata;
  data.handle = handle;
  data.destroy = destroy;

  NQMutex_lock(&looper->mutex);
  success = NQDispatchQueue_push(looper->dispatchQueue, &data);
  NQMutex_unlock(&looper->mutex);

  if (!success) {
    // TODO: in main thread
    NQ_ASSERT_NOT_REACHED();
    if (destroy)
      destroy(userdata);
    return;
  }

  NQLooper_wakeup(looper);
}

struct NQDispatchDataEx {
  NQDispatchData base;
  bool isFinished;
  NQCond cond;
  NQMutex mutex;
};

static void callAndWaitHandleCallback(void* p)
{
  struct NQDispatchDataEx* callData = (struct NQDispatchDataEx*)p;

  if (callData->base.handle)
    callData->base.handle(callData->base.userdata);

  if (callData->base.destroy)
    callData->base.destroy(callData->base.userdata);

  NQMutex_lock(&callData->mutex);
  callData->isFinished = true;
  NQCond_signal(&callData->cond);
  NQMutex_unlock(&callData->mutex);
}

void NQLooper_callAndWait(NQLooper* looper, void* userdata, NQDispatchHandleCallback handle, NQDispatchDestroyCallback destroy)
{
  bool success;

  if (NQLooper_isLoopThread(looper)) {
    if (handle)
      handle(userdata);

    if (destroy)
      destroy(userdata);

    return;
  }

  struct NQDispatchDataEx callData;

  callData.base.userdata = userdata;
  callData.base.handle = handle;
  callData.base.destroy = destroy;

  callData.isFinished = false;

  NQCond_init(&callData.cond);
  NQMutex_init(&callData.mutex);

  NQDispatchData data;
  data.userdata = &callData;
  data.handle = callAndWaitHandleCallback;
  data.destroy = NULL;

  NQMutex_lock(&looper->mutex);
  success = NQDispatchQueue_push(looper->dispatchQueue, &data);
  NQMutex_unlock(&looper->mutex);
  
  if (!success) {
    // TODO: in main thread
    NQ_ASSERT_NOT_REACHED();
    if (destroy)
      destroy(userdata);
    return;
  }

  NQLooper_wakeup(looper);

  NQMutex_lock(&callData.mutex);
  while (!callData.isFinished)
    NQCond_wait(&callData.cond, &callData.mutex);
  NQMutex_unlock(&callData.mutex);

  NQMutex_finalize(&callData.mutex);
  NQCond_finalize(&callData.cond);
}

static NQTimerIdentifier NQLooper_startTimer(NQLooper* looper, int64_t timeout, bool isInterval, void* userdata, NQDispatchHandleCallback handle, NQDispatchDestroyCallback destroy)
{
  int64_t timeoutPrev, timeoutCur;

  NQTimeMs now = NQGetTimeMs();

  NQTimerData data;
  NQTimerIdentifier id;

  data.userdata = userdata;
  data.handle = handle;
  data.destroy = destroy;

  NQMutex_lock(&looper->mutex);
  timeoutPrev = NQTimerQueue_timeout(looper->timerQueue, now);
  id = NQTimerQueue_startTimer(looper->timerQueue, isInterval, now, timeout, &data);
  timeoutCur = NQTimerQueue_timeout(looper->timerQueue, now);
  NQMutex_unlock(&looper->mutex);

  if (timeoutPrev != timeoutCur)
    NQLooper_wakeup(looper);

  return id;
}

static void NQLooper_stopTimer(NQLooper* looper, NQTimerIdentifier id)
{
  bool success;
  NQTimerData data;

  NQMutex_lock(&looper->mutex);
  success = NQTimerQueue_stopTimer(looper->timerQueue, id, &data);
  NQMutex_unlock(&looper->mutex);

  if (success && data.destroy)
    data.destroy(data.userdata);
}

NQTimerIdentifier NQLooper_callTimeout(NQLooper* looper, int64_t timeout, void* userdata, NQDispatchHandleCallback handle, NQDispatchDestroyCallback destroy)
{
  return NQLooper_startTimer(looper, timeout, false, userdata, handle, destroy);
}

void NQLooper_clearTimeout(NQLooper* looper, NQTimerIdentifier id)
{
  NQLooper_stopTimer(looper, id);
}

NQTimerIdentifier NQLooper_callInterval(NQLooper* looper, int64_t interval, void* userdata, NQDispatchHandleCallback handle, NQDispatchDestroyCallback destroy)
{
  return NQLooper_startTimer(looper, interval, true, userdata, handle, destroy);
}

void NQLooper_clearInterval(NQLooper* looper, NQTimerIdentifier id)
{
  NQLooper_stopTimer(looper, id);
}

void NQLooper_wakeup(NQLooper* looper)
{
  looper->notified = true;

  if (looper->ops.wake)
    looper->ops.wake(looper);
}

static void looperStop(void* ptr)
{
  NQLooper* looper = (NQLooper*)ptr;
  looper->ops.stop(looper);
}

void NQLooper_stop(NQLooper* looper)
{
  if (looper->ops.stop)
    NQLooper_call(looper, looper, looperStop, NULL);
}

bool NQLooper_attachSource(NQLooper* looper, NQLooperSource* source)
{
  if (looper->ops.attachSource)
    return looper->ops.attachSource(looper, source);
  return false;
}

bool NQLooper_detachSource(NQLooper* looper, NQLooperSource* source)
{
  if (looper->ops.detachSource)
    return looper->ops.detachSource(looper, source);
  return false;
}
