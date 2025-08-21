/*
 * MIT License
 *
 * Copyright (c) 2021-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#define NQ_CLASS_NAME "NQDispatchQueue"
#define NQ_LOG_TAG NQ_CLASS_NAME

#include "config.h"
#include "libnetq/DispatchQueue.h"

#include <libnetq/ObjectClass.h>
#include <libnetq/Limits.h>
#include <libnetq/Math.h>
#include <libnetq/Malloc.h>
#include <libnetq/Assert.h>

#define NQ_DISPATCH_QUEUE_MAX (NQ_UINT32_MAX - 1)

extern const NQObjectClass __NQDispatchQueueClass;

typedef struct NQDispatchDataEntry {
  struct NQDispatchDataEntry* next;
  NQDispatchData data;
} NQDispatchDataEntry;

struct NQDispatchQueue {
  const NQObjectClass* class;
  NQDispatchDataEntry* firstEntry;
  NQDispatchDataEntry* lastEntry;
  NQDispatchDataEntry* freeEntry;
  uint32_t size;
  uint32_t total;
};

NQDispatchQueue* NQDispatchQueue_create(size_t capacity)
{
  NQDispatchQueue* queue = (NQDispatchQueue*)NQMalloc(sizeof(NQDispatchQueue));
  if (queue == NULL)
    return NULL;

  queue->class = &__NQDispatchQueueClass;
  queue->firstEntry = NULL;
  queue->lastEntry = NULL;
  queue->freeEntry = NULL;
  queue->size = 0;
  queue->total = 0;

  capacity = NQGetMin(capacity, NQ_DISPATCH_QUEUE_MAX);

  size_t index;
  for (index = 0; index < capacity; index++) {
    NQDispatchDataEntry* entry = (NQDispatchDataEntry*)NQMalloc(sizeof(NQDispatchDataEntry));
    if (entry == NULL)
      break;
    entry->next = queue->freeEntry;
    queue->freeEntry = entry;
    queue->total++;
  }

  return queue;
}

static void NQDispatchDataEntryDestroy(NQDispatchDataEntry* entry, bool withDestroy)
{
  NQDispatchDataEntry* next;
  while (entry != NULL) {
    next = entry->next;

    if (withDestroy && entry->data.destroy)
      entry->data.destroy(entry->data.userdata);

    NQFree(entry);
    entry = next;
  }
}

void NQDispatchQueue_destroy(NQDispatchQueue* queue)
{
  NQ_ASSERT(queue->firstEntry == NULL);
  NQDispatchDataEntryDestroy(queue->firstEntry, true);
  NQDispatchDataEntryDestroy(queue->freeEntry, false);
  NQFree(queue);
}

size_t NQDispatchQueue_size(const NQDispatchQueue* queue)
{
  return queue->size;
}

bool NQDispatchQueue_isEmpty(const NQDispatchQueue* queue)
{
  return queue->size == 0;
}

bool NQDispatchQueue_push(NQDispatchQueue* queue, NQDispatchData* data)
{
  NQDispatchDataEntry* entry;
  
  if (queue->freeEntry) {
    entry = queue->freeEntry;
    queue->freeEntry = entry->next;
  }
  else {
    if (NQ_DISPATCH_QUEUE_MAX <= queue->total + 1)
      return false;

    entry = (NQDispatchDataEntry*)NQMalloc(sizeof(NQDispatchDataEntry));
    if (entry == NULL)
      return false;

    queue->total++;
  }

  entry->next = NULL;
  entry->data = *data;

  if (queue->lastEntry != NULL)
    queue->lastEntry->next = entry;
  else {
    NQ_ASSERT(queue->firstEntry == NULL);
    queue->firstEntry = entry;
  }

  queue->lastEntry = entry;
  queue->size++;

  return true;
}

static NQDispatchDataEntry* NQDispatchQueue_shiftEntry(NQDispatchQueue* queue)
{
  if (queue->size == 0)
    return NULL;

  NQDispatchDataEntry* entry = queue->firstEntry;
  if (queue->size != 1)
    queue->firstEntry = entry->next;
  else {
    NQ_ASSERT(queue->firstEntry == queue->lastEntry);
    queue->firstEntry = NULL;
    queue->lastEntry = NULL;
  }

  queue->size--;
  
  entry->next = queue->freeEntry;
  queue->freeEntry = entry;

  return entry;
}

bool NQDispatchQueue_shift(NQDispatchQueue* queue, NQDispatchData* data)
{
  NQDispatchDataEntry* entry = NQDispatchQueue_shiftEntry(queue);
  if (entry == NULL)
    return false;

  if (data != NULL)
    *data = entry->data;

  return true;
}

bool NQDispatchQueue_performOnce(NQDispatchQueue* queue)
{
  NQDispatchDataEntry* entry = NQDispatchQueue_shiftEntry(queue);
  if (entry == NULL)
    return false;

  if (entry->data.handle)
    entry->data.handle(entry->data.userdata);

  if (entry->data.destroy)
    entry->data.destroy(entry->data.userdata);

  return true;
}

size_t NQDispatchQueue_performAll(NQDispatchQueue* queue)
{
  size_t result = 0;
  while (NQDispatchQueue_performOnce(queue))
    result++;
  return result;
}

bool NQDispatchQueue_cleanOnce(NQDispatchQueue* queue)
{
  NQDispatchDataEntry* entry = NQDispatchQueue_shiftEntry(queue);
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

const NQObjectClass __NQDispatchQueueClass = {
  NQDispatchQueueObjectType,
  NQ_CLASS_NAME,
  NQ_VERSION_CODE,
  (NQObjectReleaseCallback)NQDispatchQueue_destroy,
};
