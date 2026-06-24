/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_WEB_WEBRESTENVAPI_H
#define _LIBNETQ_WEB_WEBRESTENVAPI_H

#include <libnetq/web/WebServer.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQWebRestEnvApi NQWebRestEnvApi;
typedef struct NQWebRestEnvParams NQWebRestEnvParams;
struct NQWebRestEnvParams {
  const char* baseUrl;
};

NQ_EXPORT NQWebRestEnvApi* NQWebRestEnvApiCreate(NQWebServer* server, struct NQWebRestEnvParams*);
NQ_EXPORT void NQWebRestEnvApiDestroy(NQWebServer* server, NQWebRestEnvApi*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_WEB_WEBRESTENVAPI_H */
