/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_WEB_WEBDIRACCESS_H
#define _LIBNETQ_WEB_WEBDIRACCESS_H

#include <libnetq/web/WebServer.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQWebDirAccessApi NQWebDirAccessApi;
typedef struct NQWebDirAccessParams NQWebDirAccessParams;

struct NQWebDirAccessParams {
  const char* baseDir;
  const char* baseUrl;
};

NQ_EXPORT NQWebDirAccessApi* NQWebDirAccessCreate(NQWebServer* server, const struct NQWebDirAccessParams*);
NQ_EXPORT void NQWebDirAccessDestroy(NQWebServer* server, NQWebDirAccessApi*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_WEB_WEBDIRACCESS_H */
