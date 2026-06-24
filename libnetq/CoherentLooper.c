/*
 * MIT License
 *
 * Copyright (c) 2023-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/CoherentLooper.h"

#include <libnetq/Malloc.h>
#include <libnetq/Cond.h>
#include <libnetq/Mutex.h>
#include <libnetq/Limits.h>
#include <libnetq/Assert.h>

struct CoherentLooperEntry {
  struct CoherentLooperEntry* next;
  NQCoherentLooperData data;
};

struct NQCoherentLooper {
  NQCond cond;
  NQMutex mutex;

  bool stopLoop;
  uint16_t totalSize;
  uint16_t queueSize;
  uint16_t freeSize;

  struct CoherentLooperEntry* first;
  struct CoherentLooperEntry* last;
  struct CoherentLooperEntry* free;
};

NQCoherentLooper* NQCoherentLooper_create(void)
{
  NQCoherentLooper* thiz = (NQCoherentLooper*)NQMalloc(sizeof(NQCoherentLooper));
  if (thiz == NULL)
    return NULL;

  NQCond_init(&thiz->cond);
  NQMutex_init(&thiz->mutex);

  thiz->stopLoop = false;

  thiz->totalSize = 0;
  thiz->queueSize = 0;
  thiz->freeSize = 0;

  thiz->first = NULL;
  thiz->last = NULL;
  thiz->free = NULL;

  return thiz;
}

static void deleteEntryChain(struct CoherentLooperEntry* entry)
{
  struct CoherentLooperEntry* temp;

  while (entry != NULL) {
    temp = entry;
    entry = entry->next;
    NQFree(temp);
  }
}

void NQCoherentLooper_destroy(NQCoherentLooper* thiz)
{
  deleteEntryChain(thiz->first);
  deleteEntryChain(thiz->free);

  NQMutex_finalize(&thiz->mutex);
  NQCond_finalize(&thiz->cond);

  NQFree(thiz);
}

void NQCoherentLooper_stop(NQCoherentLooper* thiz)
{
  NQMutex_lock(&thiz->mutex);
  if (!thiz->stopLoop) {
    thiz->stopLoop = true;
    NQCond_broadcast(&thiz->cond);
  }
  NQMutex_unlock(&thiz->mutex);
}

bool NQCoherentLooper_call(NQCoherentLooper* thiz, void* userdata, NQExecuteCallback* execute, NQDestroyCallback* destroy)
{
  if (execute == NULL) {
    NQ_ASSERT_NOT_REACHED();
    return false;
  }

  uint16_t totalSize;
  struct CoherentLooperEntry* entry = NULL;

  NQMutex_lock(&thiz->mutex);
  totalSize = thiz->totalSize;
  if (thiz->free) {
    entry = thiz->free;
    thiz->free = entry->next;

    thiz->freeSize--;

    if (thiz->first == NULL) {
      thiz->first = entry;
      thiz->last = entry;
    }
    else {
      entry->next = thiz->last;
      thiz->last = entry;
    }

    thiz->queueSize++;

    entry->next = NULL;
    entry->data.userdata = userdata;
    entry->data.execute = execute;
    entry->data.destroy = destroy;

    NQCond_signal(&thiz->cond);
  }
  NQMutex_unlock(&thiz->mutex);

  if (entry == NULL) {
    if (totalSize == NQ_UINT16_MAX) {
      NQ_ASSERT_NOT_REACHED();
      return false;
    }

    entry = (struct CoherentLooperEntry*)NQMalloc(sizeof(struct CoherentLooperEntry));
    if (entry == NULL) {
      NQ_ASSERT_NOT_REACHED();
      return false;
    }

    entry->data.userdata = userdata;
    entry->data.execute = execute;
    entry->data.destroy = destroy;

    NQMutex_lock(&thiz->mutex);
    thiz->totalSize++;
    entry->next = thiz->last;
    if (thiz->first == NULL)
      thiz->first = entry;
    thiz->last = entry;
    NQCond_signal(&thiz->cond);
    NQMutex_unlock(&thiz->mutex);
  }

  return true;
}

bool NQCoherentLooper_wait(NQCoherentLooper* thiz, NQCoherentLooperData* data)
{
  bool ret = false;

  NQMutex_lock(&thiz->mutex);

  while (!thiz->stopLoop) {
    struct CoherentLooperEntry* entry = thiz->first;
    if (entry) {
      *data = entry->data;

      if (thiz->first == thiz->last) {
        thiz->first = NULL;
        thiz->last = NULL;
      }
      else {
        thiz->first = thiz->first->next;
      }

      thiz->queueSize--;

      entry->next = thiz->free;
      thiz->free = entry;

      thiz->freeSize++;
      ret = true;
      break;
    }
    NQCond_wait(&thiz->cond, &thiz->mutex);
  }

  NQMutex_unlock(&thiz->mutex);

  return ret;
}
