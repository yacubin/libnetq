/*
 * MIT License
 *
 * Copyright (c) 2022-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#define NQ_CLASS_NAME "NQKeyVal"
#define NQ_LOG_TAG NQ_CLASS_NAME

#include "config.h"
#include "libnetq/KeyVal.h"

#include <libnetq/CStrBase.h>
#include <libnetq/ObjectClass.h>
#include <libnetq/Hash.h>
#include <libnetq/Malloc.h>
#include <libnetq/Limits.h>
#include <libnetq/Assert.h>

struct NQKeyValIter {
  struct NQKeyValIter* next;
  uint32_t klenz;
  uint32_t hash;
};

struct NQKeyVal {
  const NQObjectClass* class;
  uint32_t mask;
  uint32_t count;
  NQKeyValIter data[1];
};

#define kNQKeyValDataCount (1 << 6)

extern const NQObjectClass __NQKeyValClass;

static inline char* iter2key(NQKeyValIter* iter)
{
  return (char*)(iter) + sizeof(*iter);
}

NQKeyVal* NQKeyVal_create(void)
{
  NQKeyVal* thiz;

  thiz = (NQKeyVal*)NQMalloc(sizeof(*thiz) + sizeof(thiz->data) * (kNQKeyValDataCount - 1));
  if (thiz == NULL)
    return NULL;

  thiz->class = &__NQKeyValClass;
  thiz->mask = kNQKeyValDataCount - 1;
  thiz->count = 0;

  for (size_t i = 0; i < kNQKeyValDataCount; i++) {
    thiz->data[i].klenz = 0;
    thiz->data[i].hash = 0;
    thiz->data[i].next = ((i + 1) == kNQKeyValDataCount) ? NULL : &thiz->data[i + 1];
  }

  return thiz;
}

void NQKeyVal_destroy(NQKeyVal* thiz)
{
  NQKeyValIter* iter = &thiz->data[0];
  while (thiz->count != 0) {
    NQKeyValIter* next = iter->next;
    if (iter->klenz != 0) {
      NQFree(iter);
      thiz->count--;
    }
    iter = next;
  }
  NQFree(thiz);
}

const char* NQKeyVal_get(NQKeyVal* thiz, const char* key)
{
  uint32_t hash = NQHashCString(key);
  uint32_t index = hash & thiz->mask;
  NQKeyValIter* iter = thiz->data[index].next;
  while (iter != NULL && iter->klenz != 0) {
    if (iter->hash == hash) {
      const char* kptr = key;
      const char* ptr = iter2key(iter);
      while (*ptr == *kptr) {
        if (*ptr++ == '\0') {
          return ptr;
        }
        kptr++;
      }
    }
    iter = iter->next;
  }
  return NULL;
}

bool NQKeyVal_set(NQKeyVal* thiz, const char* key, const char* val)
{
  size_t klen = strlen(key);
  if (NQ_UINT32_MAX <= klen)
    return false;

  size_t klenz = klen + 1;
  size_t vlenz = strlen(val) + 1;

  NQKeyValIter* iter;
  iter = (NQKeyValIter*)NQMalloc(sizeof(*iter) + klenz + vlenz);
  if (iter == NULL)
    return false;

  iter->klenz = (uint32_t)klenz;

  char* payload = iter2key(iter);
  memcpy(payload, key, klenz);
  memcpy(payload + klenz, val, vlenz);

  iter->hash = NQHashString(key, klen);
  uint32_t index = iter->hash & thiz->mask;

  NQKeyValIter* next = thiz->data[index].next;
  NQKeyValIter* prev = iter;
  iter->next = next;
  thiz->data[index].next = prev;
  while (next != NULL && next->klenz != 0) {
    if ((next->hash == iter->hash) && strcmp(iter2key(next), key) == 0) {
      prev->next = next->next;
      NQFree(next);
      return true;
    }
    prev = next;
    next = next->next;
  }

  thiz->count++;
  return true;
}

NQKeyValIter* NQKeyVal_begin(NQKeyVal* thiz)
{
  if (thiz->count) {
    return NQKeyValIter_next(&thiz->data[0]);
  }
  return NULL;
}

NQKeyValIter* NQKeyValIter_next(NQKeyValIter* iter)
{
  for(;;) {
    iter = iter->next;
    if (iter == NULL || iter->klenz != 0)
      break;
  }
  return iter;
}

const char* NQKeyValIter_key(NQKeyValIter* iter)
{
  return iter2key(iter);
}

const char* NQKeyValIter_val(NQKeyValIter* iter)
{
  return iter2key(iter) + iter->klenz;
}

const NQObjectClass __NQKeyValClass = {
  kNQKeyValObjectType,
  NQ_CLASS_NAME,
  NQ_VERSION_CODE,
  (NQObjectReleaseCallback)NQKeyVal_destroy,
};
