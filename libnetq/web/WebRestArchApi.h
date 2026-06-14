/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_WEB_WEBRESTARCHAPI_H
#define _LIBNETQ_WEB_WEBRESTARCHAPI_H

#include <libnetq/web/WebServer.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQWebRestArchApi NQWebRestArchApi;
typedef struct NQWebRestArchParams NQWebRestArchParams;

struct NQWebRestArchParams {
  const char* url;
};

NQ_EXPORT NQWebRestArchApi* NQWebRestArchApiCreate(NQWebServer* server, struct NQWebRestArchParams*);
NQ_EXPORT void NQWebRestArchApiDestroy(NQWebServer* server, NQWebRestArchApi*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_WEB_WEBRESTARCHAPI_H */
