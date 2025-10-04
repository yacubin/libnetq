/*
 * MIT License
 *
 * Copyright (c) 2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#define NQ_CLASS_NAME "NQStringList"
#define NQ_LOG_TAG NQ_CLASS_NAME

#include "config.h"
#include "libnetq/StringList.h"

#include <libnetq/CStrBase.h>
#include <libnetq/Malloc.h>
#include <libnetq/Limits.h>
#include <libnetq/Log.h>

void NQStringList_finalize(NQStringList* thiz)
{
  struct NQStringEntry* iter = thiz->first;
  while (iter != NULL) {
    struct NQStringEntry* next = iter->next;
    NQFree(iter);
    iter = next;
  }
}

bool NQStringList_append(NQStringList* thiz, const char* characters)
{
  struct NQStringEntry* entry;

  size_t length = strlen(characters);
  if (NQ_UINT32_MAX < length) {
    NQ_LOGE("Length exceeded %llu", (unsigned long long)length);
    return false;
  }

  entry = (struct NQStringEntry*)NQMalloc(sizeof(*entry) + length);
  if (entry == NULL) {
    NQ_LOGE("No Memory");
    return false;
  }

  entry->next = NULL;
  entry->length = (uint32_t)length;
  memcpy(entry->characters, characters, length + 1);

  if (thiz->last == NULL) {
    thiz->first = entry;
    thiz->last = entry;
  }
  else {
    thiz->last->next = entry;
    thiz->last = entry;
  }

  return true;
}
