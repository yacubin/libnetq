/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_WEB_WEBFILEVIEW_H
#define _LIBNETQ_WEB_WEBFILEVIEW_H

#include <libnetq/web/WebServer.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQWebFileView NQWebFileView;
typedef struct NQWebFileViewParams NQWebFileViewParams;

struct NQWebFileViewParams {
  const char* baseUrl;
  const char* baseDir;
};

NQ_EXPORT NQWebFileView* NQWebFileViewCreate(NQWebServer* server, const struct NQWebFileViewParams*);
NQ_EXPORT void NQWebFileViewDestroy(NQWebServer* server, NQWebFileView*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_WEB_WEBFILEVIEW_H */
