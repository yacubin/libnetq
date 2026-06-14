/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_WEB_USERAPIEXECUTOR_H
#define _LIBNETQ_WEB_USERAPIEXECUTOR_H

#include <libnetq/web/WebServer.h>
#include <libnetq/sql/SQLite.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQWebRestUsersApi NQWebRestUsersApi;
typedef struct NQWebRestUsersParams NQWebRestUsersParams;

struct NQWebRestUsersParams {
  const char* signupUrl;
  const char* loginUrl;
  const char* databasePath;
};

NQ_EXPORT NQWebRestUsersApi* NQWebRestUsersApiCreate(NQWebServer* server, const struct NQWebRestUsersParams*);
NQ_EXPORT void NQWebRestUsersApiDestroy(NQWebServer* server, NQWebRestUsersApi*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_WEB_USERAPIEXECUTOR_H */
