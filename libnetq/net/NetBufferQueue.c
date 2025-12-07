/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/net/NetBufferQueue.h"

#include <libnetq/String.h>

void NQNetBufferQueue_init(NQNetBufferQueue* thiz)
{
  NQListHead_init(&thiz->usedList);
  NQListHead_init(&thiz->freeList);

  for (size_t i = 0; i < SPY_ENTRY_MAX; i++) {
    thiz->entries[i].size = 0;
    NQListHead_init(&thiz->entries[i].list);
    NQListHead_addBack(&thiz->freeList, &thiz->entries[i].list);
  }
}

NQNetBufferEntry* NQNetBufferQueue_alloc(NQNetBufferQueue* thiz)
{
  if (NQListHead_isEmpty(&thiz->freeList))
    return NULL;

  NQNetBufferEntry* entry = NQ_CONTAINER_OF(thiz->freeList.next, NQNetBufferEntry, list);
  NQListHead_remove(&entry->list);
  NQListHead_addBack(&thiz->usedList, &entry->list);

  return entry;
}

const NQNetBufferEntry* NQNetBufferQueue_shift(NQNetBufferQueue* thiz)
{
  if (NQListHead_isEmpty(&thiz->usedList))
    return NULL;

  NQNetBufferEntry* entry = NQ_CONTAINER_OF(thiz->usedList.next, NQNetBufferEntry, list);
  NQListHead_remove(&entry->list);
  NQListHead_addBack(&thiz->freeList, &entry->list);
  return entry;
}

bool NQNetBufferQueue_isEmpty(const NQNetBufferQueue* thiz)
{
  return NQListHead_isEmpty(&thiz->usedList);
}
