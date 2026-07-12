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
bool NQLooperSource_getInputQueue(const NQLooperSource* thiz, AInputQueue** result)
{
  if (thiz->type == NQ_SOURCE_AINPUT) {
    *result = thiz->u.getInputQueue(thiz->userdata);
    return true;
  }

  return false;
}
#endif

#ifdef NQ_OS_WINDOWS
bool NQLooperSource_getNativeHandle(const NQLooperSource* thiz, HANDLE* result)
{
  if (thiz->type == NQ_SOURCE_HANDLE) {
    *result = thiz->u.getNativeHandle(thiz->userdata);
    return true;
  }

  return false;
}
#endif

bool NQLooperSource_getFileDescriptor(const NQLooperSource* thiz, int* result)
{
  if (thiz->type == NQ_SOURCE_FD) {
    *result = thiz->u.getFileDescriptor(thiz->userdata);
    return true;
  }

  return false;
}

bool NQLooperSource_getSocketHandle(const NQLooperSource* thiz, NQSocketHandle* result)
{
  if (thiz->type == NQ_SOURCE_SOCKET) {
    *result = thiz->u.getSocketHandle(thiz->userdata);
    return true;
  }

  return false;
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
