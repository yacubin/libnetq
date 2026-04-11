/*
 * MIT License
 *
 * Copyright (c) 2020-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_NET_SPYSERVER_H
#define _LIBNETQ_NET_SPYSERVER_H

#include <libnetq/net/SpyModule.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQSpyServerOptions {
  const char* listenAddress;
  const char* targetAddress;
} NQSpyServerOptions;

typedef struct NQSpyServer NQSpyServer;

NQ_EXPORT NQSpyServer* NQSpyServer_create(const NQSpyServerOptions* options);
NQ_EXPORT void NQSpyServer_destroy(NQSpyServer* server);

NQ_EXPORT int NQSpyServer_run(NQSpyServer* server);
NQ_EXPORT bool NQSpyServer_addModule(NQSpyServer* server, const NQSpyModule* module);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_NET_SPYSERVER_H */
