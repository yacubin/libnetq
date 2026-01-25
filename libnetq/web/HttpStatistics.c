/*
 * MIT License
 *
 * Copyright (c) 2023-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/web/HttpStatistics.h"

#include <libnetq/String.h>
#include <libnetq/Malloc.h>
#include <libnetq/Limits.h>
#include <libnetq/Hash.h>
#include <libnetq/UnlimitedRandom.h>

#define kItemDataCount (1 << 6)

struct WebStatisticIter {
  struct WebStatisticIter* next;
  uint32_t ulen;
  uint32_t hash;
};

struct WebStatisticData {
  struct WebStatisticIter base;
  volatile uint32_t counter;
  char method[8];
  char url[1];
};

struct NQHttpStatistics {
  uint32_t mask;
  uint32_t count;
  struct WebStatisticIter data[1];
};

NQHttpStatistics* NQHttpStatistics_create(void)
{
  NQHttpStatistics* thiz;

  thiz = (NQHttpStatistics*)NQMalloc(sizeof(*thiz) + sizeof(thiz->data) * (kItemDataCount - 1));
  if (thiz == NULL)
    return NULL;

  thiz->mask = kItemDataCount - 1;
  thiz->count = 0;

  for (size_t i = 0; i < kItemDataCount; i++) {
    thiz->data[i].ulen = 0;
    thiz->data[i].hash = 0;
    thiz->data[i].next = ((i + 1) == kItemDataCount) ? NULL : &thiz->data[i + 1];
  }

  return thiz;
}

void NQHttpStatistics_destroy(NQHttpStatistics* thiz)
{
  struct WebStatisticIter* iter = &thiz->data[0];
  while (thiz->count != 0) {
    struct WebStatisticIter* next = iter->next;
    if (iter->ulen != 0) {
      NQFree(iter);
      thiz->count--;
    }
    iter = next;
  }
  NQFree(thiz);
}

bool NQHttpStatistics_add(NQHttpStatistics* thiz, const char* method, const char* url)
{
  struct WebStatisticData* curr;
  size_t mlen = strlen(method);
  if (mlen == 0 || sizeof(curr->method) < mlen)
    return false;

  size_t ulen = strlen(url);
  if (ulen == 0 || NQ_UINT32_MAX < ulen)
    return false;

  struct WebStatisticData* item;
  item = (struct WebStatisticData*)NQMalloc(sizeof(*item) + ulen);
  if (item == NULL)
    return false;

  item->counter = 0;
  memcpy(item->method, method, mlen + 1);
  item->base.ulen = (uint32_t)ulen;
  memcpy(item->url, url, ulen + 1);
  item->base.hash = NQHashString(url, ulen);
  uint32_t index = item->base.hash & thiz->mask;

  struct WebStatisticIter* next = thiz->data[index].next;
  struct WebStatisticIter* prev = &item->base;
  item->base.next = next;
  thiz->data[index].next = prev;
  while (next != NULL && next->ulen != 0) {
    if ((next->hash == item->base.hash) && (next->ulen == ulen)) {
      curr = (struct WebStatisticData*)next;
      if (memcmp(curr->method, method, mlen) == 0 && memcmp(curr->url, url, ulen) == 0) {
        prev->next = next->next;
        NQFree(next);
        return true;
      }
    }
    prev = next;
    next = next->next;
  }

  thiz->count++;
  return true;
}

bool NQHttpStatistics_inc(NQHttpStatistics* thiz, const char* method, const char* url)
{
  struct WebStatisticData* curr;

  size_t mlen = strlen(method);
  if (mlen == 0 || sizeof(curr->method) < mlen)
    return false;

  size_t ulen = strlen(url);
  if (ulen == 0)
    return false;

  uint32_t hash = NQHashString(url, ulen);
  uint32_t index = hash & thiz->mask;

  struct WebStatisticIter* iter = thiz->data[index].next;
  while (iter != NULL && iter->ulen != 0) {
    if (iter->hash == hash && iter->ulen == ulen) {
      curr = (struct WebStatisticData*)iter;
      if (memcmp(curr->method, method, mlen) == 0 && memcmp(curr->url, url, ulen) == 0) {
        curr->counter++;
        return true;
      }
    }
    iter = iter->next;
  }

  return false;
}

bool NQHttpStatistics_writeTo(NQHttpStatistics* thiz, NQJSONWriter* writer)
{
  if (!NQJSONWriter_writeArrayBegin(writer))
    return false;

  for (struct WebStatisticIter* iter = thiz->data[0].next; iter != NULL ;iter = iter->next) {
    if (iter->ulen != 0) {
      struct WebStatisticData* curr = (struct WebStatisticData*)iter;
      if (!NQJSONWriter_writeObjectBegin(writer))
        return false;

      if (!NQJSONWriter_writeKeyString(writer, "url", curr->url))
        return false;

      if (!NQJSONWriter_writeKeyString(writer, "method", curr->method))
        return false;

      if (!NQJSONWriter_writeKeyUint32(writer, "counter", curr->counter))
        return false;

      if (!NQJSONWriter_writeObjectEnd(writer))
        return false;
    }
  }

  if (!NQJSONWriter_writeArrayEnd(writer))
    return false;

  return true;
}
