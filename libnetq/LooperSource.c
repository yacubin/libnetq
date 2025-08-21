/*
 * MIT License
 *
 * Copyright (c) 2021-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/LooperSource.h"

#include <libnetq/Looper.h>
#include <libnetq/Malloc.h>

#ifdef NQ_OS_ANDROID
AInputQueue* NQLooperSource_getInputQueue(const NQLooperSource* thiz)
{
  if (thiz->type == NQ_SOURCE_AINPUT)
    return thiz->u.getInputQueue(thiz->userdata);

  return NULL;
}
#endif

#ifdef NQ_OS_WINDOWS
HANDLE NQLooperSource_getNativeHandle(const NQLooperSource* thiz)
{
  if (thiz->type == NQ_SOURCE_HANDLE)
    return thiz->u.getNativeHandle(thiz->userdata);

  return INVALID_HANDLE_VALUE;
}
#endif

int NQLooperSource_getFileDescriptor(const NQLooperSource* thiz)
{
  if (thiz->type == NQ_SOURCE_FD)
    return thiz->u.getFileDescriptor(thiz->userdata);

  return -1;
}

NQSocketHandle NQLooperSource_getSocketHandle(const NQLooperSource* thiz)
{
  if (thiz->type == NQ_SOURCE_SOCKET)
    return thiz->u.getSocketHandle(thiz->userdata);

  return NQ_INVALID_SOCKET;
}

void NQLooperSource_destroy(NQLooperSource* thiz)
{
  NQFree(thiz);
}

void NQLooperSource_handleEvent(NQLooperSource* thiz, const NQEvent* e)
{
  if (thiz->handleEvent)
    thiz->handleEvent(thiz->userdata, e);
}
