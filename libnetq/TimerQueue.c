/*
 * MIT License
 *
 * Copyright (c) 2020-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#define NQ_CLASS_NAME "NQTimerQueue"
#define NQ_LOG_TAG NQ_CLASS_NAME

#include "config.h"
#include "libnetq/TimerQueue.h"

#include <libnetq/string/CStrBase.h>
#include <libnetq/List.h>
#include <libnetq/Malloc.h>
#include <libnetq/Assert.h>
#include <libnetq/BitOps.h>

#define NQ_TIMER_ACTIVE   (1 << 0)
#define NQ_TIMER_INTERVAL (1 << 1)

typedef struct NQTimerEntry NQTimerEntry;
typedef struct NQTimerList NQTimerList;

struct NQTimerEntry {
  NQListHead list;

  uint32_t flags;
  NQTimerIdentifier id;

  int64_t timeout;
  int64_t timePoint;

  NQTimerData data;
};

struct NQTimerQueue {
  NQListHead activeList;
  NQListHead freeList;
  uint32_t allocCount;
  NQTimerEntry allocTimer[1];
};

static inline NQTimerEntry* toTimerEntry(NQListHead* list)
{
  return NQ_CONTAINER_OF(list, NQTimerEntry, list);
}

static void NQTimerList_insert(NQListHead* list, NQTimerEntry* entry)
{
  if (NQListHead_isEmpty(list))
    NQListHead_addBack(list, &entry->list);
  else {
    NQListHead* iter = list->next;
    for (;;) {
      NQTimerEntry* curr = toTimerEntry(iter);
      if (entry->timePoint < curr->timePoint) {
        NQListHead_addBack(&curr->list, &entry->list);;
        break;
      }
      if (iter == list) {
        NQListHead_addBack(list, &entry->list);
        break;
      }
      iter = iter->next;
    }
  }
}

static void NQTimerQueue_init(NQTimerQueue* thiz, uint32_t allocCount)
{
  uint32_t index;

  thiz->allocCount = allocCount;
  NQListHead_init(&thiz->activeList);
  NQListHead_init(&thiz->freeList);

  for (index = 0; index < allocCount; index++) {
    NQTimerEntry* entry = &thiz->allocTimer[index];
    entry->id = index + 1;
    NQListHead_addBack(&thiz->freeList, &entry->list);
  }
}

NQTimerQueue* NQTimerQueue_create(size_t maxSize)
{
  if (maxSize == 0)
    return NULL;

  unsigned n = NQGetFls64(maxSize);
  if (n > 16)
    return NULL;

  uint32_t allocCount = (1u << n);
  NQTimerQueue* thiz = (NQTimerQueue*)NQZeroMalloc(sizeof(NQTimerQueue) + sizeof(NQTimerEntry) * (allocCount - 1));
  if (thiz == NULL)
    return NULL;

  NQTimerQueue_init(thiz, allocCount);
  return thiz;
}

void NQTimerQueue_destroy(NQTimerQueue* thiz)
{
  NQFree(thiz);
}

int NQTimerQueue_timeout(NQTimerQueue* thiz, int64_t now)
{
  if (NQListHead_isEmpty(&thiz->activeList))
    return -1;

  NQTimerEntry* entry = toTimerEntry(thiz->activeList.next);
  int64_t t = entry->timePoint;
  if (now < t)
    return (int)(t - now);

  return 0;
}

int NQTimerQueue_shiftTimer(NQTimerQueue* thiz, int64_t now, NQTimerData* data)
{
  if (NQListHead_isEmpty(&thiz->activeList))
    return -1;

  NQTimerEntry* entry = toTimerEntry(thiz->activeList.next);
  NQ_ASSERT(entry->flags & NQ_TIMER_ACTIVE);

  if (now == -1) {
    if (data != NULL)
      *data = entry->data;

    NQListHead_remove(&entry->list);
    entry->id += thiz->allocCount;
    entry->flags &= ~NQ_TIMER_ACTIVE;
    NQListHead_addBack(&thiz->freeList, &entry->list);

    return 0;
  }

  int64_t t = entry->timePoint;
  int64_t timeout = (now < t) ? (t - now) : 0;
  if (timeout > 0)
    return (int)timeout;

  NQListHead_remove(&entry->list);

  bool isInterval = (entry->flags & NQ_TIMER_INTERVAL) != 0;
  if (data != NULL) {
    data->userdata = entry->data.userdata;
    data->handle = entry->data.handle;
    data->destroy = isInterval ? NULL : entry->data.destroy;
  }

  if (isInterval) {
    entry->timePoint += entry->timeout;
    NQTimerList_insert(&thiz->activeList, entry);
  }
  else {
    entry->id += thiz->allocCount;
    entry->flags &= ~NQ_TIMER_ACTIVE;
    NQListHead_addBack(&thiz->freeList, &entry->list);
  }
  
  return 0;
}

NQTimerIdentifier NQTimerQueue_startTimer(NQTimerQueue* thiz, bool isInterval, int64_t now, int64_t timeout, NQTimerData* data)
{
  if (data == NULL || timeout <= 0)
    return NQ_TIMER_INVALID;

  if (NQListHead_isEmpty(&thiz->freeList))
    return NQ_TIMER_INVALID;

  NQTimerEntry* entry = toTimerEntry(thiz->freeList.next);
  NQListHead_remove(&entry->list);
  entry->flags = NQ_TIMER_ACTIVE;
  if (isInterval)
    entry->flags |= NQ_TIMER_INTERVAL;

  entry->timeout = timeout;
  entry->timePoint = entry->timeout + now;

  entry->data = *data;

  NQTimerList_insert(&thiz->activeList, entry);

  return entry->id;
}

static NQTimerEntry* NQTimerQueue_findEntryById(NQTimerQueue* thiz, NQTimerIdentifier identifier)
{
  if (identifier == NQ_TIMER_INVALID)
    return NULL;

  size_t index = (identifier - 1) & (thiz->allocCount - 1);
  NQTimerEntry* entry = &thiz->allocTimer[index];
  if (entry->id != identifier)
    return NULL;

  return entry;
}

bool NQTimerQueue_stopTimer(NQTimerQueue* thiz, NQTimerIdentifier identifier, NQTimerData* data)
{
  NQTimerEntry* entry = NQTimerQueue_findEntryById(thiz, identifier);
  if (entry == NULL || !(entry->flags & NQ_TIMER_ACTIVE))
    return false;

  if (data != NULL)
    *data = entry->data;

  NQListHead_remove(&entry->list);
  entry->id += thiz->allocCount;
  entry->flags &= ~NQ_TIMER_ACTIVE;
  NQListHead_addBack(&thiz->freeList, &entry->list);
  
  return true;
}

bool NQTimerQueue_isValid(NQTimerQueue* thiz, NQTimerIdentifier identifier)
{
  return NQTimerQueue_findEntryById(thiz, identifier) != NULL;
}
