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

#ifdef NQ_OS_WIN

#include <windows.h>

#include <libnetq/Malloc.h>
#include <libnetq/Assert.h>
  
#define WM_USER_DISPATCH (WM_USER + 1)

struct Looper {
  HANDLE eventHandle[1];
  DWORD eventTotal;
  bool hasMessage;
};

static int Looper_init(NQLooper* looper)
{
  struct Looper* thiz = NQMalloc(sizeof(struct Looper));

  memset(thiz->eventHandle, 0, sizeof(thiz->eventHandle));
  thiz->eventTotal = 0;
  thiz->hasMessage = false;

  looper->priv = thiz;

  return 0;
}

static void Looper_finalize(NQLooper* looper)
{
  struct Looper* thiz = (struct Looper*)looper->priv;
  NQFree(thiz);
}

static void Looper_wake(NQLooper* looper)
{
  BOOL result;

  if (NQLooper_isLoopThread(looper))
    result = PostMessageW(NULL, WM_USER_DISPATCH, 0, 0);
  else
    result = PostThreadMessageW((DWORD)looper->tid, WM_USER_DISPATCH, 0, 0);

  NQ_ASSERT_UNUSED(result, result);
}

static void Looper_stop(NQLooper* looper)
{
  PostQuitMessage(0);
}

static int Looper_poll(NQLooper* looper, int64_t timeout)
{
  struct Looper* thiz = (struct Looper*)looper->priv;

  DWORD dwMilliseconds;
  DWORD waitResult;

  if (thiz->hasMessage)
    return NQ_LOOPER_POLL_MESSAGE;

  dwMilliseconds = (timeout == -1) ? INFINITE : (DWORD)(timeout);
  waitResult = MsgWaitForMultipleObjectsEx(thiz->eventTotal, thiz->eventHandle, dwMilliseconds, QS_ALLINPUT | QS_ALLPOSTMESSAGE, MWMO_ALERTABLE | MWMO_INPUTAVAILABLE);

  if (waitResult == WAIT_FAILED)
    return NQ_LOOPER_POLL_ERROR;

  if (waitResult == WAIT_TIMEOUT)
    return NQ_LOOPER_POLL_TIMEOUT;

  if (waitResult == thiz->eventTotal) {
    thiz->hasMessage = true;
  }
  else if (WAIT_OBJECT_0 <= waitResult && waitResult <= WAIT_OBJECT_0 + thiz->eventTotal) {
    // Source
  }

  return NQ_LOOPER_POLL_MESSAGE;
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
  struct Looper* thiz = (struct Looper*)looper->priv;

  MSG msg;

  if (!thiz->hasMessage)
    return false;

  while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {

    if (msg.message == WM_QUIT)
      looper->stopLoop = true;
    else if (msg.message == WM_USER_DISPATCH) {
      message->type = NQ_MESSAGE_WAKEUP;
      return true;
    }

    TranslateMessage(&msg);
    DispatchMessageW(&msg);
  }

  thiz->hasMessage = false;
  return false;
}

const struct NQLooperOperations WindowsLooperOperations = {
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
