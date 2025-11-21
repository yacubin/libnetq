/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#define NQ_CLASS_NAME "NQTimerQueue"
#define NQ_LOG_TAG NQ_CLASS_NAME

#include "config.h"
#include "libnetq/TimerQueue.h"

#include <libnetq/CStrBase.h>
#include <libnetq/List.h>
#include <libnetq/Malloc.h>
#include <libnetq/Assert.h>

#define NQ_TIMER_MAX (1 << 10)
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
  NQTimerEntry allocTimer[NQ_TIMER_MAX];
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

static void NQTimerQueue_init(NQTimerQueue* thiz)
{
  uint32_t index;

  memset(thiz, 0, sizeof(*thiz));

  NQListHead_init(&thiz->activeList);
  NQListHead_init(&thiz->freeList);

  for (index = 0; index < NQ_ARRAY_LENGTH(thiz->allocTimer); index++) {
    NQTimerEntry* entry = &thiz->allocTimer[index];
    entry->id = index + 1;
    NQListHead_addBack(&thiz->freeList, &entry->list);
  }
}

NQTimerQueue* NQTimerQueue_create()
{
  NQTimerQueue* thiz = (NQTimerQueue*)NQMalloc(sizeof(NQTimerQueue));
  if (thiz == NULL)
    return NULL;
  NQTimerQueue_init(thiz);
  return thiz;
}

void NQTimerQueue_destroy(NQTimerQueue* thiz)
{
  NQFree(thiz);
}

int64_t NQTimerQueue_timeout(NQTimerQueue* thiz, int64_t now)
{
  if (NQListHead_isEmpty(&thiz->activeList))
    return -1;

  NQTimerEntry* entry = toTimerEntry(thiz->activeList.next);
  int64_t t = entry->timePoint;
  if (now < t)
    return t - now;

  return 0;
}

static int NQTimerQueue_stopNearTimer(NQTimerQueue* thiz, NQTimerData* data)
{
  if (NQListHead_isEmpty(&thiz->activeList))
    return NQ_TIMER_WAIT;

  NQTimerEntry* entry = toTimerEntry(thiz->activeList.next);
  NQ_ASSERT(entry->flags & NQ_TIMER_ACTIVE);
  if (data != NULL)
    *data = entry->data;
  
  NQListHead_remove(&entry->list);
  entry->id += NQ_TIMER_MAX;
  entry->flags &= ~NQ_TIMER_ACTIVE;
  NQListHead_addBack(&thiz->freeList, &entry->list);
  
  return NQ_TIMER_REMOVE;
}

int NQTimerQueue_nextFired(NQTimerQueue* thiz, int64_t now, NQTimerData* data)
{
  if (now == -1)
    return NQTimerQueue_stopNearTimer(thiz, data);

  int64_t timeout = NQTimerQueue_timeout(thiz, now);
  if (timeout != 0)
    return NQ_TIMER_WAIT;

  NQ_ASSERT(NQListHead_isEmpty(&thiz->activeList) == false);
  NQTimerEntry* entry = toTimerEntry(thiz->activeList.next);
  NQ_ASSERT(entry->flags & NQ_TIMER_ACTIVE);
  NQListHead_remove(&entry->list);

  if (data != NULL)
    *data = entry->data;

  int status;
  if (entry->flags & NQ_TIMER_INTERVAL) {
    entry->timePoint += entry->timeout;
    NQTimerList_insert(&thiz->activeList, entry);
    status = NQ_TIMER_REPEATE;
  }
  else {
    entry->id += NQ_TIMER_MAX;
    entry->flags &= ~NQ_TIMER_ACTIVE;
    NQListHead_addBack(&thiz->freeList, &entry->list);
    status = NQ_TIMER_REMOVE;
  }
  
  return status;
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

  size_t index = (identifier - 1) & (NQ_TIMER_MAX - 1);
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
  entry->id += NQ_TIMER_MAX;
  entry->flags &= ~NQ_TIMER_ACTIVE;
  NQListHead_addBack(&thiz->freeList, &entry->list);
  
  return true;
}

bool NQTimerQueue_isValid(NQTimerQueue* thiz, NQTimerIdentifier identifier)
{
  return NQTimerQueue_findEntryById(thiz, identifier) != NULL;
}
