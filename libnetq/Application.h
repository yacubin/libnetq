/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_APPLICATION_H
#define _LIBNETQ_APPLICATION_H

#include <libnetq/Thread.h>
#include <libnetq/Mutex.h>
#include <libnetq/Cond.h>
#include <libnetq/EventTarget.h>
#include <libnetq/ApplicationCallbacks.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQApplication NQApplication;
typedef struct NQAssetSystem NQAssetSystem;

typedef struct NQApplicationOperations NQApplicationOperations;
struct NQApplication {
  void* priv;

  const NQApplicationOperations* operations;
  NQApplicationCallbacks* callbacks;

  int mode;
  bool running;
  bool destroyed;

  NQThread* thread;
  NQMutex mutex;
  NQCond cond;

  NQEventTarget eventTarget;
};

struct NQApplicationOperations {
  int (*init) (NQApplication* app);
  void (*finalize) (NQApplication* app);
  void (*dispatch) (NQApplication* app);
  int (*handleEvent) (NQApplication* app, const NQEvent* e);
};

enum {
  NQ_APP_FLAG_SYNC = 0,
  NQ_APP_FLAG_ASYNC = (1 << 0),
};

NQ_EXPORT void NQApplication_init(NQApplication* app, const NQApplicationOperations* ops, NQApplicationCallbacks* callbacks);
NQ_EXPORT int NQApplication_run(NQApplication* app, int flags);
NQ_EXPORT void NQApplication_finalize(NQApplication* app);

NQ_EXPORT void NQApplication_sendEvent(NQApplication* app, const NQEvent* e);

// EventTarget
NQ_EXPORT void NQApplication_addEventListener(NQApplication*, NQEventCallback* callback, void* userdata);
NQ_EXPORT void NQApplication_removeEventListener(NQApplication*, NQEventCallback* callback);
NQ_EXPORT bool NQApplication_dispatchEvent(NQApplication*, const NQEvent* e);

NQ_EXPORT void* NQApplication_getUserPtr(NQApplication* app);
NQ_EXPORT void NQApplication_setUserPtr(NQApplication* app, void* userdata);

NQ_EXPORT NQApplication* NQGetMainApplication(void);
NQ_EXPORT void NQApplicationInitialize(const NQApplicationOperations* ops, NQApplicationCallbacks* callbacks);
NQ_EXPORT void NQApplicationSendEvent(const NQEvent* e);
NQ_EXPORT int NQApplicationRun(int flags);
NQ_EXPORT void NQApplicationShutdown(void);

NQ_EXPORT int NQMainApplicationRun(const NQApplicationOperations* ops, NQApplicationCallbacks* callbacks);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_APPLICATION_H */
