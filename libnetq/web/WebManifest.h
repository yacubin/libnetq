/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_WEB_WEBMANIFEST_H
#define _LIBNETQ_WEB_WEBMANIFEST_H

#include <libnetq/web/WebServer.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NQ_WEBMANIFEST_FILE "manifest.json"

typedef struct NQWebManifestExecutor NQWebManifestExecutor;

NQ_EXPORT NQWebManifestExecutor* NQWebManifestExecutorCreate(NQWebServer* server, const char* filename);
NQ_EXPORT void NQWebManifestExecutorDestroy(NQWebServer* server, NQWebManifestExecutor* manifest);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_WEB_WEBMANIFEST_H */
