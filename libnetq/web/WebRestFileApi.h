/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_WEB_WEBRESTFILEAPI_H
#define _LIBNETQ_WEB_WEBRESTFILEAPI_H

#include <libnetq/web/WebServer.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQWebRestFileApi NQWebRestFileApi;
typedef struct NQWebRestFileParams NQWebRestFileParams;

struct NQWebRestFileParams {
  const char* baseUrl;
  const char* baseDir;
};

NQ_EXPORT NQWebRestFileApi* NQWebRestFileApiCreate(NQWebServer* server, const struct NQWebRestFileParams*);
NQ_EXPORT void NQWebRestFileApiDestroy(NQWebServer* server, NQWebRestFileApi*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_WEB_WEBRESTFILEAPI_H */
