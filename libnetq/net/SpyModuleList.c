/*
 * MIT License
 *
 * Copyright (c) 2020-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/net/SpyModuleList.h"

#include <libnetq/Malloc.h>

void NQSpyModuleList_init(NQSpyModuleList* list)
{
  NQListHead_init(&list->listHead);
}

void NQSpyModuleList_finalize(NQSpyModuleList* list)
{
  NQListHead* iter = list->listHead.next;
  while (iter != &list->listHead) {
    struct NQSpyModuleEntry* entry = NQ_CONTAINER_OF(iter, struct NQSpyModuleEntry, list);
    iter = iter->next;
    NQFree(entry);
  }
}

bool NQSpyModuleList_add(NQSpyModuleList* list, const NQSpyModule* module)
{
  struct NQSpyModuleEntry* entry = (struct NQSpyModuleEntry*)NQMalloc(sizeof(struct NQSpyModuleEntry) + module->callbacks->sizeInBytes);
  if (entry == NULL)
    return false;

  NQListHead_init(&entry->list);
  entry->module = *module;
  entry->flags = 0;

  NQListHead_addBack(&list->listHead, &entry->list);

  return true;
}

void NQSpyModuleList_doInit(NQSpyModuleList* list)
{
  for (NQListHead* iter = list->listHead.next; iter != &list->listHead; iter = iter->next) {
    struct NQSpyModuleEntry* entry = NQ_CONTAINER_OF(iter, struct NQSpyModuleEntry, list);
    if (entry->module.callbacks->onInit)
      entry->module.callbacks->onInit(&entry->module);
  }
}

void NQSpyModuleList_doRelease(NQSpyModuleList* list)
{
  for (NQListHead* iter = list->listHead.next; iter != &list->listHead; iter = iter->next) {
    struct NQSpyModuleEntry* entry = NQ_CONTAINER_OF(iter, struct NQSpyModuleEntry, list);
    if (entry->module.callbacks->onRelease)
      entry->module.callbacks->onRelease(&entry->module);
  }
}

void NQSpyModuleList_doClientConnect(NQSpyModuleList* list, const NQSpyClient* client)
{
  for (NQListHead* iter = list->listHead.next; iter != &list->listHead; iter = iter->next) {
    struct NQSpyModuleEntry* entry = NQ_CONTAINER_OF(iter, struct NQSpyModuleEntry, list);
    if (entry->module.callbacks->onClientConnect)
      entry->module.callbacks->onClientConnect(&entry->module, client);
  }
}

void NQSpyModuleList_doClientClose(NQSpyModuleList* list, const NQSpyClient* client)
{
  for (NQListHead* iter = list->listHead.next; iter != &list->listHead; iter = iter->next) {
    struct NQSpyModuleEntry* entry = NQ_CONTAINER_OF(iter, struct NQSpyModuleEntry, list);
    if (entry->module.callbacks->onClientClose)
      entry->module.callbacks->onClientClose(&entry->module, client);
  }
}

void NQSpyModuleList_doClientReceive(NQSpyModuleList* list, const NQSpyClient* client, const uint8_t* data, size_t size)
{
  for (NQListHead* iter = list->listHead.next; iter != &list->listHead; iter = iter->next) {
    struct NQSpyModuleEntry* entry = NQ_CONTAINER_OF(iter, struct NQSpyModuleEntry, list);
    if (entry->module.callbacks->onClientReceive)
      entry->module.callbacks->onClientReceive(&entry->module, client, data, size);
  }
}

void NQSpyModuleList_doClientSend(NQSpyModuleList* list, const NQSpyClient* client, const uint8_t* data, size_t size)
{
  for (NQListHead* iter = list->listHead.next; iter != &list->listHead; iter = iter->next) {
    struct NQSpyModuleEntry* entry = NQ_CONTAINER_OF(iter, struct NQSpyModuleEntry, list);
    if (entry->module.callbacks->onClientSend)
      entry->module.callbacks->onClientSend(&entry->module, client, data, size);
  }
}

void NQSpyModuleList_doRemoteConnect(NQSpyModuleList* list, const NQSpyClient* target)
{
  for (NQListHead* iter = list->listHead.next; iter != &list->listHead; iter = iter->next) {
    struct NQSpyModuleEntry* entry = NQ_CONTAINER_OF(iter, struct NQSpyModuleEntry, list);
    if (entry->module.callbacks->onRemoteConnect)
      entry->module.callbacks->onRemoteConnect(&entry->module, target);
  }
}

void NQSpyModuleList_doRemoteClose(NQSpyModuleList* list, const NQSpyClient* target)
{
  for (NQListHead* iter = list->listHead.next; iter != &list->listHead; iter = iter->next) {
    struct NQSpyModuleEntry* entry = NQ_CONTAINER_OF(iter, struct NQSpyModuleEntry, list);
    if (entry->module.callbacks->onRemoteClose)
      entry->module.callbacks->onRemoteClose(&entry->module, target);
  }
}

void NQSpyModuleList_doRemoteReceive(NQSpyModuleList* list, const NQSpyClient* target, const uint8_t* data, size_t size)
{
  for (NQListHead* iter = list->listHead.next; iter != &list->listHead; iter = iter->next) {
    struct NQSpyModuleEntry* entry = NQ_CONTAINER_OF(iter, struct NQSpyModuleEntry, list);
    if (entry->module.callbacks->onRemoteReceive)
      entry->module.callbacks->onRemoteReceive(&entry->module, target, data, size);
  }
}

void NQSpyModuleList_doRemoteSend(NQSpyModuleList* list, const NQSpyClient* target, const uint8_t* data, size_t size)
{
  for (NQListHead* iter = list->listHead.next; iter != &list->listHead; iter = iter->next) {
    struct NQSpyModuleEntry* entry = NQ_CONTAINER_OF(iter, struct NQSpyModuleEntry, list);
    if (entry->module.callbacks->onRemoteSend)
      entry->module.callbacks->onRemoteSend(&entry->module, target, data, size);
  }
}
