/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_WEB_WEBFILEURLMAP_H
#define _LIBNETQ_WEB_WEBFILEURLMAP_H

#include <libnetq/web/WebServer.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQWebFileUrlMapApi NQWebFileUrlMapApi;
typedef struct NQWebFileUrlMapParams NQWebFileUrlMapParams;

struct NQWebFileUrlMapItem {
  const char* file;
  const char* url;
};

struct NQWebFileUrlMapParams {
  const char* baseDir;
  const char* baseUrl;
  struct NQWebFileUrlMapItem* items;
};

NQ_EXPORT NQWebFileUrlMapApi* NQWebFileUrlMapCreate(NQWebServer* server, const struct NQWebFileUrlMapParams*);
NQ_EXPORT void NQWebFileUrlMapDestroy(NQWebServer* server, NQWebFileUrlMapApi*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_WEB_WEBFILEURLMAP_H */
