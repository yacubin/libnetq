/*
 * MIT License
 *
 * Copyright (c) 2020-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_SPYMODULELIST_H
#define _LIBNETQ_SPYMODULELIST_H

#include <libnetq/List.h>
#include <libnetq/net/SpyModule.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQSpyModuleList NQSpyModuleList;

struct NQSpyModuleEntry {
  NQListHead list;
  int flags;

  NQSpyModule module;
};

struct NQSpyModuleList {
  NQListHead listHead;
};

NQ_EXPORT void NQSpyModuleList_init(NQSpyModuleList* list);
NQ_EXPORT void NQSpyModuleList_finalize(NQSpyModuleList* list);

NQ_EXPORT bool NQSpyModuleList_add(NQSpyModuleList* list, const NQSpyModule* module);

NQ_EXPORT void NQSpyModuleList_doInit(NQSpyModuleList* list);
NQ_EXPORT void NQSpyModuleList_doRelease(NQSpyModuleList* list);
NQ_EXPORT void NQSpyModuleList_doClientConnect(NQSpyModuleList* list, const NQSpyClient* client);
NQ_EXPORT void NQSpyModuleList_doClientClose(NQSpyModuleList* list, const NQSpyClient* client);
NQ_EXPORT void NQSpyModuleList_doClientReceive(NQSpyModuleList* list, const NQSpyClient* client, const uint8_t* data, size_t size);
NQ_EXPORT void NQSpyModuleList_doClientSend(NQSpyModuleList* list, const NQSpyClient* client, const uint8_t* data, size_t size);
NQ_EXPORT void NQSpyModuleList_doRemoteConnect(NQSpyModuleList* list, const NQSpyClient* target);
NQ_EXPORT void NQSpyModuleList_doRemoteClose(NQSpyModuleList* list, const NQSpyClient* target);
NQ_EXPORT void NQSpyModuleList_doRemoteReceive(NQSpyModuleList* list, const NQSpyClient* target, const uint8_t* data, size_t size);
NQ_EXPORT void NQSpyModuleList_doRemoteSend(NQSpyModuleList* list, const NQSpyClient* target, const uint8_t* data, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_SPYMODULELIST_H */
