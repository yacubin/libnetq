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

#include <libnetq/ObjectClass.h>
#include <libnetq/CStrBase.h>
#include <libnetq/Malloc.h>
#include <libnetq/Assert.h>

extern const NQObjectClass __NQTimerQueueClass;

#define NQ_TIMER_MAX (1 << 10)
#define NQ_TIMER_ACTIVE   (1 << 0)
#define NQ_TIMER_INTERVAL (1 << 1)

typedef struct NQTimerEntry NQTimerEntry;
typedef struct NQTimerList NQTimerList;

struct NQTimerEntry {
  NQTimerEntry* prev;
  NQTimerEntry* next;

  uint32_t flags;
  NQTimerIdentifier id;

  int64_t timeout;
  int64_t timePoint;

  NQTimerData data;
};

struct NQTimerList {
  NQTimerEntry* first;
  NQTimerEntry* last;
  size_t size;
};

struct NQTimerQueue {
  const NQObjectClass* class;
  NQTimerList activeList;
  NQTimerList freeList;
  NQTimerEntry allocTimer[NQ_TIMER_MAX];
};

static void NQTimerList_init(NQTimerList* list)
{
  list->first = NULL;
  list->last = NULL;
  list->size = 0;
}

static NQTimerEntry* NQTimerList_remove(NQTimerList* list, NQTimerEntry* entry)
{
  if (entry->next != NULL)
    entry->next->prev = entry->prev;
  else {
    NQ_ASSERT(list->last == entry);
    list->last = entry->prev;
  }

  if (entry->prev != NULL)
    entry->prev->next = entry->next;
  else {
    NQ_ASSERT(list->first == entry);
    list->first = entry->next;
  }

  list->size--;

  return entry;
}

static NQTimerEntry* NQTimerList_shift(NQTimerList* list)
{
  if (list->first == NULL)
    return NULL;

  NQTimerEntry* entry = list->first;

  list->first = entry->next;

  if (list->first != NULL)
    list->first->prev = NULL;
  else
    list->last = NULL;

  list->size--;

  return entry;
}

NQ_ALLOW_UNUSED
static NQTimerEntry* NQTimerList_pop(NQTimerList* list)
{
  if (list->last == NULL)
    return NULL;

  NQTimerEntry* entry = list->last;

  list->last = entry->prev;

  if (list->last != NULL)
    list->last->next = NULL;

  list->size--;

  return entry;
}

NQ_ALLOW_UNUSED
static void NQTimerList_unshift(NQTimerList* list, NQTimerEntry* entry)
{
  entry->next = list->first;

  if (list->first != NULL) {
    NQ_ASSERT(list->first->prev == NULL);
    list->first->prev = entry;
  }
  else {
    NQ_ASSERT(list->last == NULL);
    list->last = entry;
  }

  entry->prev = NULL;
  list->first = entry;
  list->size++;
}

static void NQTimerList_push(NQTimerList* list, NQTimerEntry* entry)
{
  entry->prev = list->last;

  if (list->last != NULL)
    list->last->next = entry;
  else {
    NQ_ASSERT(list->size == 0);
    NQ_ASSERT(list->first == NULL);
    list->first = entry;
  }

  entry->next = NULL;
  list->last = entry;
  list->size++;
}

static void NQTimerList_insertBefore(NQTimerList* list, NQTimerEntry* position, NQTimerEntry* entry)
{
  if (list->first == position) {
    list->first = entry;
  }
  else {
    position->prev->next = entry;
  }

  entry->prev = position->prev;
  entry->next = position;
  position->prev = entry;
  list->size++;
}

NQ_ALLOW_UNUSED
static void NQTimerList_insertAfter(NQTimerList* list, NQTimerEntry* position, NQTimerEntry* entry)
{
  position->next = entry;
  entry->prev = position;
  if (list->last == position)
    list->last = entry;
  list->size++;
}

static void NQTimerList_insert(NQTimerList* list, NQTimerEntry* entry)
{
  NQTimerEntry* iter = list->first;
  if (iter == NULL)
    NQTimerList_push(list, entry);
  else {
    for (;;) {
      if (entry->timePoint < iter->timePoint) {
        NQTimerList_insertBefore(list, iter, entry);
        break;
      }
      if (iter->next == NULL) {
        NQTimerList_push(list, entry);
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

  thiz->class = &__NQTimerQueueClass;

  NQTimerList_init(&thiz->activeList);
  NQTimerList_init(&thiz->freeList);

  for (index = 0; index < NQ_ARRAY_LENGTH(thiz->allocTimer); index++) {
    NQTimerEntry* entry = &thiz->allocTimer[index];
    entry->id = index + 1;
    NQTimerList_push(&thiz->freeList, entry);
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
  if (thiz->activeList.size == 0)
    return -1;

  int64_t t = thiz->activeList.first->timePoint;
  if (now < t)
    return t - now;

  return 0;
}

static int NQTimerQueue_stopNearTimer(NQTimerQueue* thiz, NQTimerData* data)
{
  NQTimerEntry* entry = NQTimerList_shift(&thiz->activeList);
  if (entry == NULL)
    return NQ_TIMER_WAIT;
  
  NQ_ASSERT(entry->flags & NQ_TIMER_ACTIVE);
  if (data != NULL)
    *data = entry->data;
  
  NQTimerList_remove(&thiz->activeList, entry);
  entry->id += NQ_TIMER_MAX;
  entry->flags &= ~NQ_TIMER_ACTIVE;
  NQTimerList_push(&thiz->freeList, entry);
  
  return NQ_TIMER_REMOVE;
}

int NQTimerQueue_nextFired(NQTimerQueue* thiz, int64_t now, NQTimerData* data)
{
  if (now == -1)
    return NQTimerQueue_stopNearTimer(thiz, data);

  int64_t timeout = NQTimerQueue_timeout(thiz, now);
  if (timeout != 0)
    return NQ_TIMER_WAIT;

  NQTimerEntry* entry = NQTimerList_shift(&thiz->activeList);
  NQ_ASSERT(entry->flags & NQ_TIMER_ACTIVE);

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
    NQTimerList_push(&thiz->freeList, entry);
    status = NQ_TIMER_REMOVE;
  }
  
  return status;
}

NQTimerIdentifier NQTimerQueue_startTimer(NQTimerQueue* thiz, bool isInterval, int64_t now, int64_t timeout, NQTimerData* data)
{
  if (data == NULL || timeout <= 0)
    return NQ_TIMER_INVALID;

  NQTimerEntry* entry = NQTimerList_shift(&thiz->freeList);
  if (entry == NULL)
    return NQ_TIMER_INVALID;

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

  NQTimerList_remove(&thiz->activeList, entry);
  entry->id += NQ_TIMER_MAX;
  entry->flags &= ~NQ_TIMER_ACTIVE;
  NQTimerList_push(&thiz->freeList, entry);
  
  return true;
}

bool NQTimerQueue_isValid(NQTimerQueue* thiz, NQTimerIdentifier identifier)
{
  return NQTimerQueue_findEntryById(thiz, identifier) != NULL;
}

const NQObjectClass __NQTimerQueueClass = {
  NQTimerQueueObjectType,
  NQ_CLASS_NAME,
  NQ_VERSION_CODE,
  (NQObjectReleaseCallback)NQTimerQueue_destroy,
};
