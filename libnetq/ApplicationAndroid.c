/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/ApplicationAndroid.h"

#ifdef NQ_OS_ANDROID

void NQApplication_doActivityCreate(NQApplication* app, ANativeActivity* activity, void* savedState, size_t savedStateSize)
{
  NQActivityCreateEvent event = {
    NQ_EVENT_ACTIVITY_CREATE,
    activity,
    savedState,
    savedStateSize,
  };

  NQApplication_sendEvent(app, (NQEvent*)&event);
}

void NQApplication_doActivityWindowCreated(NQApplication* app, ANativeActivity* activity, ANativeWindow* window)
{
  NQActivityWindowEvent event = {
    NQ_EVENT_ACTIVITY_WINDOW_CREATED,
    activity,
    window,
  };

  NQApplication_sendEvent(app, (NQEvent*)&event);
}

void NQApplication_doActivityWindowResized(NQApplication* app, ANativeActivity* activity, ANativeWindow* window)
{
  NQActivityWindowEvent event = {
    NQ_EVENT_ACTIVITY_WINDOW_RESIZED,
    activity,
    window,
  };

  NQApplication_sendEvent(app, (NQEvent*)&event);
}

void NQApplication_doActivityWindowRedrawNeeded(NQApplication* app, ANativeActivity* activity, ANativeWindow* window)
{
  NQActivityWindowEvent event = {
    NQ_EVENT_ACTIVITY_WINDOW_REDRAW_NEEDED,
    activity,
    window,
  };

  NQApplication_sendEvent(app, (NQEvent*)&event);
}

void NQApplication_doActivityWindowDestroyed(NQApplication* app, ANativeActivity* activity, ANativeWindow* window)
{
  NQActivityWindowEvent event = {
    NQ_EVENT_ACTIVITY_WINDOW_DESTROYED,
    activity,
    window,
  };

  NQApplication_sendEvent(app, (NQEvent*)&event);
}

void NQApplication_doActivityInputQueueCreated(NQApplication* app, ANativeActivity* activity, AInputQueue* queue)
{
  NQActivityInputQueueEvent event = {
    NQ_EVENT_ACTIVITY_INPUT_QUEUE_CREATED,
    activity,
    queue,
  };

  NQApplication_sendEvent(app, (NQEvent*)&event);
}

void NQApplication_doActivityInputQueueDestroyed(NQApplication* app, ANativeActivity* activity, AInputQueue* queue)
{
  NQActivityInputQueueEvent event = {
    NQ_EVENT_ACTIVITY_INPUT_QUEUE_DESTROYED,
    activity,
    queue,
  };

  NQApplication_sendEvent(app, (NQEvent*)&event);
}

#endif /* NQ_OS_ANDROID */
