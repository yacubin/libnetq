/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_WEB_WEBENVVIEW_H
#define _LIBNETQ_WEB_WebEnvView_H

#include <libnetq/web/WebServer.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQWebEnvView NQWebEnvView;
typedef struct NQWebEnvViewParams NQWebEnvViewParams;

struct NQWebEnvViewParams {
  const char* baseUrl;
  bool allowEdit;
};

NQ_EXPORT NQWebEnvView* NQWebEnvViewCreate(NQWebServer* server, const struct NQWebEnvViewParams*);
NQ_EXPORT void NQWebEnvViewDestroy(NQWebServer* server, NQWebEnvView*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_WEB_WebEnvView_H */
