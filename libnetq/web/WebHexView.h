/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_WEB_WEBHEXVIEW_H
#define _LIBNETQ_WEB_WEBHEXVIEW_H

#include <libnetq/web/WebServer.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQWebHexView NQWebHexView;
typedef struct NQWebHexViewParams NQWebHexViewParams;

struct NQWebHexViewParams {
  const char* baseUrl;
};

NQ_EXPORT NQWebHexView* NQWebHexViewCreate(NQWebServer* server, const struct NQWebHexViewParams*);
NQ_EXPORT void NQWebHexViewDestroy(NQWebServer* server, NQWebHexView*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_WEB_WEBHEXVIEW_H */
