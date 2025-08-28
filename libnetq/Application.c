/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/Application.h"

#include <string.h>

#include <libnetq/Looper.h>

enum {
  NQ_MODE_NONE,
  NQ_MODE_SYNC,
  NQ_MODE_ASYNC,
};

static void NQApplication_doCreate(NQApplication* app);
static void NQApplication_doDestroy(NQApplication* app);

static NQApplication s_application;

void NQApplication_init(NQApplication* app, const NQApplicationOperations* ops, NQApplicationCallbacks* callbacks)
{
  app->priv = NULL;
  app->operations = ops;
  app->callbacks = callbacks;

  app->mode = NQ_MODE_NONE;

  app->running = false;
  app->destroyed = false;

  app->thread = NULL;
  NQMutex_init(&app->mutex);
  NQCond_init(&app->cond);

  NQEventTarget_init(&app->eventTarget);

  if (app->operations && app->operations->init)
    app->operations->init(app);
}

void NQApplication_finalize(NQApplication* app)
{
  if (app->mode == NQ_MODE_ASYNC) {
    NQMutex_lock(&app->mutex);
    while (!app->destroyed)
      NQCond_wait(&app->cond, &app->mutex);
    NQMutex_unlock(&app->mutex);
  }

  if (app->operations && app->operations->finalize)
    app->operations->finalize(app);

  NQEventTarget_finalize(&app->eventTarget);

  NQCond_destroy(&app->cond);
  NQMutex_destroy(&app->mutex);
}

static void NQApplication_worker(void* p)
{
  NQApplication* app = (NQApplication*)p;

  NQApplication_doCreate(app);

  NQMutex_lock(&app->mutex);
  app->running = true;
  NQCond_broadcast(&app->cond);
  NQMutex_unlock(&app->mutex);

  if (app->operations->dispatch)
    app->operations->dispatch(app);

  NQApplication_doDestroy(app);

  NQMutex_lock(&app->mutex);
  app->destroyed = true;
  NQCond_broadcast(&app->cond);
  NQMutex_unlock(&app->mutex);
}

int NQApplication_run(NQApplication* app, int flags)
{
  if (flags & NQ_APP_FLAG_ASYNC) {
    app->mode = NQ_MODE_ASYNC;

    app->thread = NQThread_create(NQApplication_worker, app, "NQAppAsync");
    NQThread_detach(app->thread);

    NQMutex_lock(&app->mutex);
    while (!app->running)
      NQCond_wait(&app->cond, &app->mutex);
    NQMutex_unlock(&app->mutex);
  }
  else {
    app->mode = NQ_MODE_SYNC;
    NQApplication_doCreate(app);

    if (app->operations->dispatch)
      app->operations->dispatch(app);

    NQApplication_doDestroy(app);
  }

  return 0;
}

void NQApplication_doCreate(NQApplication* app)
{
  NQMainLooperInitialize(NULL);

  if (app->callbacks && app->callbacks->onCreate)
    app->callbacks->onCreate(app);
}

void NQApplication_doDestroy(NQApplication* app)
{
  if (app->callbacks && app->callbacks->onDestroy)
    app->callbacks->onDestroy(app);

  NQMainLooperShutdown();
}

void NQApplication_sendEvent(NQApplication* app, const NQEvent* e)
{
  if (app->operations->handleEvent)
    app->operations->handleEvent(app, e);
}

bool NQApplication_dispatchEvent(NQApplication* thiz, const NQEvent* e)
{
  return NQEventTarget_dispatchEvent(&thiz->eventTarget, e);
}

void NQApplication_addEventListener(NQApplication* thiz, NQEventCallback* callback, void* userdata)
{
  NQEventTarget_addEventListener(&thiz->eventTarget, callback, userdata);
}

void NQApplication_removeEventListener(NQApplication* thiz, NQEventCallback* callback)
{
  NQEventTarget_removeEventListener(&thiz->eventTarget, callback);
}

void* NQApplication_getUserPtr(NQApplication* app)
{
  return app->priv;
}

void NQApplication_setUserPtr(NQApplication* app, void* userdata)
{
  app->priv = userdata;
}

NQApplication* NQGetMainApplication()
{
  return &s_application;
}

void NQApplicationInitialize(const NQApplicationOperations* ops, NQApplicationCallbacks* callbacks)
{
  NQApplication_init(&s_application, ops, callbacks);
}

void NQApplicationSendEvent(const NQEvent* e)
{
  NQApplication_sendEvent(&s_application, e);
}

int NQApplicationRun(int flags)
{
  return NQApplication_run(&s_application, flags);
}

void NQApplicationShutdown()
{
  NQApplication_finalize(&s_application);
}

int NQMainApplicationRun(const NQApplicationOperations* ops, NQApplicationCallbacks* callbacks)
{
  NQApplicationInitialize(ops, callbacks);
  int result = NQApplicationRun(NQ_APP_FLAG_SYNC);
  NQApplicationShutdown();

  return result;
}
