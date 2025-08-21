/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_APPLICATIONANDROID_H
#define _LIBNETQ_APPLICATIONANDROID_H

#include <libnetq/Application.h>

#ifdef NQ_OS_ANDROID
#include <android/native_activity.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef NQ_OS_ANDROID

typedef struct NQActivityCreateEvent {
  int type;
  ANativeActivity* activity;
  void* savedState;
  size_t savedStateSize;
} NQActivityCreateEvent;

NQ_EXPORT void NQApplication_doActivityCreate(NQApplication* app, ANativeActivity* activity, void* savedState, size_t savedStateSize);

typedef struct NQActivityWindowEvent {
  int type;
  ANativeActivity* activity;
  ANativeWindow* window;
} NQActivityWindowEvent;

NQ_EXPORT void NQApplication_doActivityWindowCreated(NQApplication* app, ANativeActivity* activity, ANativeWindow* window);
NQ_EXPORT void NQApplication_doActivityWindowResized(NQApplication* app, ANativeActivity* activity, ANativeWindow* window);
NQ_EXPORT void NQApplication_doActivityWindowRedrawNeeded(NQApplication* app, ANativeActivity* activity, ANativeWindow* window);
NQ_EXPORT void NQApplication_doActivityWindowDestroyed(NQApplication* app, ANativeActivity* activity, ANativeWindow* window);

typedef struct NQActivityInputQueueEvent {
  int type;
  ANativeActivity* activity;
  AInputQueue* queue;
} NQActivityInputQueueEvent;

NQ_EXPORT void NQApplication_doActivityInputQueueCreated(NQApplication* app, ANativeActivity* activity, AInputQueue* queue);
NQ_EXPORT void NQApplication_doActivityInputQueueDestroyed(NQApplication* app, ANativeActivity* activity, AInputQueue* queue);

#endif /* NQ_OS_ANDROID */

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_APPLICATIONANDROID_H */
