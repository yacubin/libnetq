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

#include <libnetq/Looper.h>
#include <libnetq/PlatformLooper.h>
#include <libnetq/Cond.h>
#include <libnetq/Time.h>
#include <libnetq/Malloc.h>
#include <libnetq/Assert.h>

static NQLooper s_looper;

#if defined(NQ_OS_DARWIN)
extern const struct NQLooperOperations CoreFoundationLooperOperations;
#elif defined(NQ_OS_WINDOWS)

static int Looper_init(NQLooper* looper)
{
  NQPlatformLooper* impl = (NQPlatformLooper*)NQMalloc(sizeof(NQPlatformLooper));
  NQPlatformLooper_init(impl);
  looper->priv = impl;
  return 0;
}

static void Looper_finalize(NQLooper* looper)
{
  NQPlatformLooper* impl = (NQPlatformLooper*)looper->priv;
  NQPlatformLooper_finilize(impl);
  NQFree((void*)impl);
}

static void Looper_wake(NQLooper* looper)
{
  NQPlatformLooper* impl = (NQPlatformLooper*)looper->priv;
  NQPlatformLooper_wake(impl);
}

static void Looper_stop(NQLooper* looper)
{
  NQPlatformLooper* impl = (NQPlatformLooper*)looper->priv;
  NQPlatformLooper_stop(impl);
}

static int Looper_poll(NQLooper* looper, int64_t timeout)
{
  NQPlatformLooper* impl = (NQPlatformLooper*)looper->priv;
  return NQPlatformLooper_poll(impl, timeout);
}

static bool Looper_attachSource(NQLooper* looper, NQLooperSource* source)
{
  return false;
}

static bool Looper_detachSource(NQLooper* looper, NQLooperSource* source)
{
  return false;
}

static bool Looper_getMessage(NQLooper* looper, NQMessage* message)
{
  NQPlatformLooper* impl = (NQPlatformLooper*)looper->priv;
  return NQPlatformLooper_getMessage(impl, message);
}

static const struct NQLooperOperations WindowsLooperOperations = {
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

#elif defined(NQ_OS_ANDROID)
extern const struct NQLooperOperations AndroidLooperOperations;
#else
extern const struct NQLooperOperations SelectLooperOperations;
#endif

void NQMainLooperInitialize(const struct NQLooperOperations* ops)
{
  if (ops == NULL) {
#if defined(NQ_OS_DARWIN)
    ops = &CoreFoundationLooperOperations;
#elif defined(NQ_OS_WINDOWS)
    ops = &WindowsLooperOperations;
#elif defined(NQ_OS_ANDROID)
    ops = &AndroidLooperOperations;
#else
    ops = &SelectLooperOperations;
#endif
  }

  NQLooper_init(&s_looper, ops);
}

void NQMainLooperShutdown(void)
{
  NQLooper_finalize(&s_looper);
}

NQLooper* NQLooperGetMain(void)
{
  return &s_looper;
}
