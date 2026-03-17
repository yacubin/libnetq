/*
 * MIT License
 *
 * Copyright (c) 2021-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#define NQ_CLASS_NAME "NQDispatchQueue"
#define NQ_LOG_TAG NQ_CLASS_NAME

#include "config.h"
#include "libnetq/DispatchQueue.h"

#include <libnetq/Limits.h>
#include <libnetq/Math.h>
#include <libnetq/Malloc.h>
#include <libnetq/Assert.h>

#define NQ_DISPATCH_QUEUE_MAX (NQ_UINT32_MAX - 1)

struct DispatchDataEntry {
  NQDispatchData data;
  NQListHead list;
};

static struct DispatchDataEntry* toDispatchDataEntry(NQListHead* list)
{
  return NQ_CONTAINER_OF(list, struct DispatchDataEntry, list);
}

void NQDispatchQueue_init(NQDispatchQueue* thiz)
{

  NQListHead_init(&thiz->workList);
  NQListHead_init(&thiz->freeList);
  thiz->size = 0;
  thiz->total = 0;
}

NQDispatchQueue* NQDispatchQueue_create(size_t capacity)
{
  NQDispatchQueue* thiz = (NQDispatchQueue*)NQMalloc(sizeof(*thiz));
  if (thiz == NULL)
    return NULL;

  NQDispatchQueue_init(thiz);

  capacity = NQGetMin(capacity, NQ_DISPATCH_QUEUE_MAX);
  for (size_t index = 0; index < capacity; index++) {
    struct DispatchDataEntry* entry = (struct DispatchDataEntry*)NQMalloc(sizeof(*entry));
    if (entry == NULL)
      break;
    NQListHead_addBack(&thiz->freeList, &entry->list);
    thiz->total++;
  }

  return thiz;
}

void NQDispatchQueue_finalize(NQDispatchQueue* thiz)
{
  NQListHead* iter;

  iter = thiz->workList.next;
  while (iter != &thiz->workList) {
    struct DispatchDataEntry* entry = toDispatchDataEntry(iter);
    iter = iter->next;
    if (entry->data.destroy)
      entry->data.destroy(entry->data.userdata);
    NQFree(entry);
  }

  iter = thiz->freeList.next;
  while (iter != &thiz->freeList) {
    struct DispatchDataEntry* entry = toDispatchDataEntry(iter);
    iter = iter->next;
    NQFree(entry);
  }
}

void NQDispatchQueue_destroy(NQDispatchQueue* thiz)
{
  NQDispatchQueue_finalize(thiz);
  NQFree(thiz);
}

size_t NQDispatchQueue_size(const NQDispatchQueue* thiz)
{
  return thiz->size;
}

bool NQDispatchQueue_isEmpty(const NQDispatchQueue* thiz)
{
  return thiz->size == 0;
}

bool NQDispatchQueue_push(NQDispatchQueue* thiz, NQDispatchData* data)
{
  struct DispatchDataEntry* entry;

  if (NQListHead_isEmpty(&thiz->freeList)) {
    if (NQ_DISPATCH_QUEUE_MAX <= thiz->total + 1)
      return false;

    entry = (struct DispatchDataEntry*)NQMalloc(sizeof(*entry));
    if (entry == NULL)
      return false;

    thiz->total++;
  }
  else {
    entry = toDispatchDataEntry(thiz->freeList.next);
    NQListHead_remove(thiz->freeList.next);
  }

  entry->data = *data;
  NQListHead_addBack(&thiz->workList, &entry->list);
  thiz->size++;

  return true;
}

static struct DispatchDataEntry* NQDispatchQueue_shiftEntry(NQDispatchQueue* thiz)
{
  if (NQListHead_isEmpty(&thiz->workList)) {
    NQ_ASSERT(thiz->size == 0);
    return NULL;
  }
  else {
    NQ_ASSERT(thiz->size > 0);
    struct DispatchDataEntry* entry = toDispatchDataEntry(thiz->workList.next);
    NQListHead_remove(thiz->workList.next);
    thiz->size--;
    return entry;
  }
}

bool NQDispatchQueue_shift(NQDispatchQueue* thiz, NQDispatchData* data)
{
  struct DispatchDataEntry* entry = NQDispatchQueue_shiftEntry(thiz);
  if (entry == NULL)
    return false;

  if (data != NULL)
    *data = entry->data;

  return true;
}

bool NQDispatchQueue_performOnce(NQDispatchQueue* thiz)
{
  struct DispatchDataEntry* entry = NQDispatchQueue_shiftEntry(thiz);
  if (entry == NULL)
    return false;

  if (entry->data.handle)
    entry->data.handle(entry->data.userdata);

  if (entry->data.destroy)
    entry->data.destroy(entry->data.userdata);

  return true;
}

size_t NQDispatchQueue_performAll(NQDispatchQueue* thiz)
{
  size_t result = 0;
  while (NQDispatchQueue_performOnce(thiz))
    result++;
  return result;
}

bool NQDispatchQueue_cleanOnce(NQDispatchQueue* thiz)
{
  struct DispatchDataEntry* entry = NQDispatchQueue_shiftEntry(thiz);
  if (entry == NULL)
    return false;

  if (entry->data.destroy)
    entry->data.destroy(entry->data.userdata);

  return true;
}

size_t NQDispatchQueue_cleanAll(NQDispatchQueue* queue)
{
  size_t result = 0;
  while (NQDispatchQueue_cleanOnce(queue))
    result++;
  return result;
}
