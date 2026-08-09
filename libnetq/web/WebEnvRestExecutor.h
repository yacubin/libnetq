/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_WEB_WEBENVRESTEXECUTOR_H
#define _LIBNETQ_WEB_WEBENVRESTEXECUTOR_H

#include <libnetq/web/WebServer.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQWebEnvRestParams NQWebEnvRestParams;
typedef struct NQWebEnvRestListeners NQWebEnvRestListeners;
typedef struct NQWebEnvRestExecutor NQWebEnvRestExecutor;

struct NQWebEnvRestParams {
  const char* baseUrl;
};

struct NQWebEnvRestListeners {
  struct NQWebRequestListener allEnvListener;
  struct NQWebRequestListener getEnvListener;
  struct NQWebRequestListener setEnvListener;
};

struct NQWebEnvRestExecutor {
  NQWebExecutor executor;
  NQWebEnvRestListeners listeners;
};

NQ_EXPORT int NQWebEnvRestListenersInit(NQWebExecutor* executor, NQWebEnvRestListeners* listeners, const NQWebEnvRestParams* params);
NQ_EXPORT void NQWebEnvRestListenersFinalize(NQWebExecutor* executor, NQWebEnvRestListeners* listeners);

NQ_EXPORT NQWebEnvRestExecutor* NQWebEnvRestExecutorCreate(NQWebServer* server, struct NQWebEnvRestParams*);
NQ_EXPORT void NQWebEnvRestExecutorDestroy(NQWebServer* server, NQWebEnvRestExecutor*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_WEB_WEBENVRESTEXECUTOR_H */
