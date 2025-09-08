/*
 * MIT License
 *
 * Copyright (c) 2021-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/WindowManager.h"

#include <libnetq/Malloc.h>
#include <libnetq/Mutex.h>
#include <libnetq/UnlimitedRandom.h>
#include <libnetq/String.h>
#include <libnetq/Assert.h>

struct NQWindowManagerStorage {
  const struct NQWindowManagerOperations* vtabl;
  NQWindowManager* impl;
  struct NQWindowManagerOperations ops;
};

static bool NQWindowManager_init(NQWindowManager* thiz, void* argument)
{
  NQ_UNUSED_PARAM(thiz);
  return false;
}

static bool NQWindowManager_finalize(NQWindowManager* thiz)
{
  NQ_UNUSED_PARAM(thiz);
  return false;
}

static NQWindowHandle NQWindowManager_createWindow(NQWindowManager* thiz, void* userdata, NQWindowCallback callback)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(userdata);
  NQ_UNUSED_PARAM(callback);
  return NQWindowInvalid;
}

static bool NQWindowManager_destroyWindow(NQWindowManager* thiz, NQWindowHandle handle)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(handle);
  return false;
}

static bool NQWindowManager_ioctl(NQWindowManager* thiz, NQWindowHandle handle, int request, void* ptr)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(handle);
  NQ_UNUSED_PARAM(request);
  NQ_UNUSED_PARAM(ptr);
  return false;
}

static bool NQWindowManager_resizeTo(NQWindowManager* thiz, NQWindowHandle handle, int width, int height)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(handle);
  NQ_UNUSED_PARAM(width);
  NQ_UNUSED_PARAM(height);
  return false;
}

static int NQWindowManager_getTitle(NQWindowManager* thiz, NQWindowHandle handle, char* s, size_t n)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(handle);
  NQ_UNUSED_PARAM(s);
  NQ_UNUSED_PARAM(n);
  return -1;
}

static bool NQWindowManager_setTitle(NQWindowManager* thiz, NQWindowHandle handle, const char* s)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(handle);
  NQ_UNUSED_PARAM(s);
  return false;
}

static bool NQWindowManager_setVisible(NQWindowManager* thiz, NQWindowHandle handle, bool value)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(handle);
  NQ_UNUSED_PARAM(value);
  return false;
}

static bool NQWindowManager_moveTo(NQWindowManager* thiz, NQWindowHandle handle, int x, int y)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(handle);
  NQ_UNUSED_PARAM(x);
  NQ_UNUSED_PARAM(y);
  return false;
}

static bool NQWindowManager_clear(NQWindowManager* thiz, NQWindowHandle handle)
{
  NQ_UNUSED_PARAM(thiz);
  return false;
}

static bool NQWindowManager_setToolTip(NQWindowManager* manager, NQWindowHandle handle, const char* s)
{
  NQ_UNUSED_PARAM(manager);
  NQ_UNUSED_PARAM(handle);
  NQ_UNUSED_PARAM(s);
  return false;
}

static bool NQWindowManager_setBorderWidth(NQWindowManager* manager, NQWindowHandle handle, int width)
{
  NQ_UNUSED_PARAM(manager);
  NQ_UNUSED_PARAM(handle);
  NQ_UNUSED_PARAM(width);
  return false;
}

static struct NQWindowManagerOperations s_operations = {
  0,
  &NQWindowManager_init,
  &NQWindowManager_finalize,
  &NQWindowManager_createWindow,
  &NQWindowManager_destroyWindow,
  &NQWindowManager_ioctl,
  &NQWindowManager_resizeTo,
  &NQWindowManager_getTitle,
  &NQWindowManager_setTitle,
  &NQWindowManager_setVisible,
  &NQWindowManager_moveTo,
  &NQWindowManager_clear,
  &NQWindowManager_setToolTip,
  &NQWindowManager_setBorderWidth,
};

static struct NQWindowManagerStorage s_storage = {
  &s_operations, NULL
};

NQWindowHandle NQWindowCreate(void* userdata, NQWindowCallback callback)
{
  return s_storage.vtabl->createWindow(s_storage.impl, userdata, callback);
}

bool NQWindowDestroy(NQWindowHandle handle)
{
  return s_storage.vtabl->destroyWindow(s_storage.impl, handle);
}

bool NQWindowIoctl(NQWindowHandle handle, int request, void* ptr)
{
  return s_storage.vtabl->ioctl(s_storage.impl, handle, request, ptr);
}

bool NQWindowResizeTo(NQWindowHandle handle, int width, int height)
{
  return s_storage.vtabl->resizeTo(s_storage.impl, handle, width, height);
}

int NQWindowGetTitle(NQWindowHandle handle, char* s, size_t n)
{
  return s_storage.vtabl->getTitle(s_storage.impl, handle, s, n);
}

bool NQWindowSetTitle(NQWindowHandle handle, const char* s)
{
  return s_storage.vtabl->setTitle(s_storage.impl, handle, s);
}

bool NQWindowSetVisible(NQWindowHandle handle, bool value)
{
  return s_storage.vtabl->setVisible(s_storage.impl, handle, value);
}

bool NQWindowMoveTo(NQWindowHandle handle, int x, int y)
{
  return s_storage.vtabl->moveTo(s_storage.impl, handle, x, y);
}

bool NQWindowClear(NQWindowHandle handle)
{
  return s_storage.vtabl->clear(s_storage.impl, handle);
}

bool NQWindowSetToolTip(NQWindowHandle handle, const char* s)
{
  return s_storage.vtabl->setToolTip(s_storage.impl, handle, s);
}

bool NQWindowSetBorderWidth(NQWindowHandle handle, int width)
{
  return s_storage.vtabl->setBorderWidth(s_storage.impl, handle, width);
}

void NQWindowManagerInitialize(const struct NQWindowManagerOperations* ops, void* argument)
{
  NQ_ASSERT(s_storage.vtabl == &s_operations);

  if (ops == NULL) {
    s_storage.vtabl = &s_operations;
    return;
  }

  s_storage.impl = NULL;
  if (ops->sizeInBytes != 0) {
    s_storage.impl = NQMalloc(ops->sizeInBytes);
    if (s_storage.impl == NULL) {
      s_storage.vtabl = &s_operations;
      return;
    }
  }

  s_storage.ops = *ops;

  if (s_storage.ops.init == NULL)
    s_storage.ops.init = &NQWindowManager_init;

  if (s_storage.ops.finalize == NULL)
    s_storage.ops.finalize = &NQWindowManager_finalize;

  if (s_storage.ops.createWindow == NULL)
    s_storage.ops.createWindow = &NQWindowManager_createWindow;

  if (s_storage.ops.destroyWindow == NULL)
    s_storage.ops.destroyWindow = &NQWindowManager_destroyWindow;

  if (s_storage.ops.ioctl == NULL)
    s_storage.ops.ioctl = &NQWindowManager_ioctl;

  if (s_storage.ops.resizeTo == NULL)
    s_storage.ops.resizeTo = &NQWindowManager_resizeTo;

  if (s_storage.ops.getTitle == NULL)
    s_storage.ops.getTitle = &NQWindowManager_getTitle;

  if (s_storage.ops.setTitle == NULL)
    s_storage.ops.setTitle = &NQWindowManager_setTitle;

  if (s_storage.ops.setVisible == NULL)
    s_storage.ops.setVisible = &NQWindowManager_setVisible;

  if (s_storage.ops.moveTo == NULL)
    s_storage.ops.moveTo = &NQWindowManager_moveTo;

  if (s_storage.ops.clear == NULL)
    s_storage.ops.clear = &NQWindowManager_clear;

  if (s_storage.ops.setToolTip == NULL)
    s_storage.ops.setToolTip = &NQWindowManager_setToolTip;

  if (s_storage.ops.setBorderWidth == NULL)
    s_storage.ops.setBorderWidth = &NQWindowManager_setBorderWidth;

  s_storage.vtabl = &s_storage.ops;

  if (!s_storage.vtabl->init(s_storage.impl, argument)) {
    s_storage.vtabl = &s_operations;
    if (s_storage.impl != NULL)
      NQFree(s_storage.impl);
    return;
  }
}

void NQWindowManagerShutdown()
{
  if (!s_storage.vtabl->finalize(s_storage.impl))
    return;
  
  s_storage.vtabl = &s_operations;
  if (s_storage.impl != NULL)
    NQFree(s_storage.impl);
}
