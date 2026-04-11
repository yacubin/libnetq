/*
 * MIT License
 *
 * Copyright (c) 2020-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_NET_SPYMODULE_H
#define _LIBNETQ_NET_SPYMODULE_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQSpyClient {
  char* address;
} NQSpyClient;

struct NQSpyModuleCallbacks;

typedef struct NQSpyModule {
  const struct NQSpyModuleCallbacks* callbacks;
} NQSpyModule;

typedef struct NQSpyModuleCallbacks {
  size_t sizeInBytes;
  void (*onInit)(NQSpyModule* module);
  void (*onRelease)(NQSpyModule* module);
  /* Client */
  void (*onClientConnect)(NQSpyModule* module, const NQSpyClient* client);
  void (*onClientClose)(NQSpyModule* module, const NQSpyClient* client);
  void (*onClientReceive)(NQSpyModule* module, const NQSpyClient* client, const uint8_t* data, size_t size);
  void (*onClientSend)(NQSpyModule* module, const NQSpyClient* client, const uint8_t* data, size_t size);
  /* Remote */
  void (*onRemoteConnect)(NQSpyModule* module, const NQSpyClient* target);
  void (*onRemoteClose)(NQSpyModule* module, const NQSpyClient* target);
  void (*onRemoteReceive)(NQSpyModule* module, const NQSpyClient* target, const uint8_t* data, size_t size);
  void (*onRemoteSend)(NQSpyModule* module, const NQSpyClient* target, const uint8_t* data, size_t size);
} NQSpyModuleCallbacks;

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_NET_SPYMODULE_H */
