/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_LOOPER_H
#define _LIBNETQ_LOOPER_H

#include <libnetq/Basic.h>
#include <libnetq/Mutex.h>
#include <libnetq/Thread.h>
#include <libnetq/Time.h>
#include <libnetq/DispatchQueue.h>
#include <libnetq/TimerQueue.h>
#include <libnetq/LooperSource.h>

#ifdef __cplusplus
extern "C" {
#endif

enum {
  NQ_LOOPER_NATIVE,
  NQ_LOOPER_SELECTFD,
  NQ_LOOPER_POLLFD,
};

enum {
  NQ_LOOPER_POLL_ERROR = -1,
  NQ_LOOPER_POLL_TIMEOUT = 0,
  NQ_LOOPER_POLL_MESSAGE = 1,
  NQ_LOOPER_POLL_SOURCE = 2,
};

typedef struct NQLooper NQLooper;

#define NQ_TIMER_ONCE (1 << 0)
#define NQ_TIMER_REPEAT (1 << 1)
#define NQ_TIMER_FIRED (1 << 2)
#define NQ_TIMER_RELEASE (1 << 3)

typedef void (*NQTimerCallback) (void* userdata, NQTimerIdentifier id, int flags);
typedef void (*NQDDetachHandleCallback) (void* userdata);
typedef void (*NQDDetachDestroyCallback) (void* userdata);

#define NQ_MESSAGE_QUIT 1
#define NQ_MESSAGE_WAKEUP 2
#define NQ_MESSAGE_NORMAL 3

typedef struct NQMessage {
  int type;
} NQMessage;

struct NQLooperOperations {
  int type;

  int  (*init)         (NQLooper* looper);
  void (*finalize)     (NQLooper* looper);

  void (*dispatch)     (NQLooper* looper);
  void (*wake)         (NQLooper* looper);
  void (*stop)         (NQLooper* looper);
  int  (*poll)         (NQLooper* looper, int64_t timeout);

  bool (*attachSource) (NQLooper* looper, NQLooperSource* source);
  bool (*detachSource) (NQLooper* looper, NQLooperSource* source);

  NQTimerIdentifier (*startTimer) (int64_t timeout, void* userdata, NQTimerCallback handle, int flags);
  void (*stopTimer) (NQTimerIdentifier id, int flags);

  bool (*getMessage)   (NQLooper* looper, NQMessage* message);
};

// Rename onto NQLooperSystem
struct NQLooper {
  NQMutex mutex;
  NQTID tid;

  bool stopLoop;
  bool notified;
  NQTimerQueue* timerQueue;
  NQDispatchQueue* dispatchQueue;
  NQTime poolTime;

  void* priv;
  struct NQLooperOperations ops;
};

NQ_EXPORT void NQLooper_init(NQLooper* looper, const struct NQLooperOperations* ops);
NQ_EXPORT void NQLooper_finalize(NQLooper* looper);

NQ_EXPORT bool NQLooper_isLoopThread(const NQLooper* looper);
NQ_EXPORT void NQLooper_dispatch(NQLooper* looper);

NQ_EXPORT void NQLooper_call(NQLooper* looper, void* userdata, NQDDetachHandleCallback handle, NQDDetachDestroyCallback destroy);
NQ_EXPORT void NQLooper_callAndWait(NQLooper* looper, void* userdata, NQDDetachHandleCallback handle, NQDDetachDestroyCallback destroy);

NQ_EXPORT NQTimerIdentifier NQLooper_callTimeout(NQLooper* looper, int64_t timeout, void* userdata, NQDDetachHandleCallback handle, NQDDetachDestroyCallback destroy);
NQ_EXPORT void NQLooper_clearTimeout(NQLooper* looper, NQTimerIdentifier id);
NQ_EXPORT NQTimerIdentifier NQLooper_callInterval(NQLooper* looper, int64_t interval, void* userdata, NQDDetachHandleCallback handle, NQDDetachDestroyCallback destroy);
NQ_EXPORT void NQLooper_clearInterval(NQLooper* looper, NQTimerIdentifier id);

NQ_EXPORT void NQLooper_wakeup(NQLooper* looper);
NQ_EXPORT void NQLooper_stop(NQLooper* looper);

NQ_EXPORT bool NQLooper_attachSource(NQLooper* looper, NQLooperSource* source);
NQ_EXPORT bool NQLooper_detachSource(NQLooper* looper, NQLooperSource* source);

NQ_EXPORT void NQMainLooperInitialize(const struct NQLooperOperations* ops);
NQ_EXPORT void NQMainLooperShutdown();

NQ_EXPORT NQLooper* NQLooperGetMain();

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_LOOPER_H */
