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

#include <libnetq/OS.h>
#include <libnetq/Looper.h>
#include <libnetq/Cond.h>
#include <libnetq/Time.h>
#include <libnetq/Malloc.h>
#include <libnetq/Assert.h>

#if defined(NQ_OS_DARWIN)
extern const struct NQLooperOperations CoreFoundationLooperOperations;
#elif defined(NQ_OS_WIN)
extern const struct NQLooperOperations WindowsLooperOperations;
#elif defined(NQ_OS_ANDROID)
extern const struct NQLooperOperations AndroidLooperOperations;
#else
extern const struct NQLooperOperations SelectLooperOperations;
#endif

static NQLooper s_looper;

static const struct NQLooperOperations* nativeOperations()
{
#if defined(NQ_OS_DARWIN)
  return &CoreFoundationLooperOperations;
#elif defined(NQ_OS_WIN)
  return &WindowsLooperOperations;
#elif defined(NQ_OS_ANDROID)
  return &AndroidLooperOperations;
#else
  return &SelectLooperOperations;
#endif
}

void NQMainLooperInitialize(const struct NQLooperOperations* ops)
{
  if (ops == NULL)
    ops = nativeOperations();

  NQLooper_init(&s_looper, ops);
}

void NQMainLooperShutdown()
{
  NQLooper_finalize(&s_looper);
}

NQLooper* NQLooperGetMain()
{
  return &s_looper;
}
