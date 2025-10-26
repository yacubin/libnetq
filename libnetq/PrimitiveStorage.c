/*
 * MIT License
 *
 * Copyright (c) 2023-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#define NQ_CLASS_NAME "NQPrimitiveStorage"
#define NQ_LOG_TAG NQ_CLASS_NAME

#include "config.h"
#include "libnetq/PrimitiveStorage.h"

#include <libnetq/ObjectClass.h>
#include <libnetq/CStrBase.h>
#include <libnetq/Malloc.h>
#include <libnetq/Math.h>
#include <libnetq/Limits.h>

enum {
  kNQPrimitiveStorageHandler = NQ_UINT32_MAX
};

struct NQPrimitiveStorageEntry {
  struct NQPrimitiveStorageEntry* next;
  const char* name;

  uint32_t type;

  union {
    uint32_t dataUint32;
    const char* dataString;
    NQPrimitiveStorageHandler* dataHandler;
  };

  union {
    size_t dataLength;
    void* dataPointer;
  };
};

void NQPrimitiveStorage_init(NQPrimitiveStorage* thiz, NQPrimitiveStorage* parent)
{
  thiz->parent = parent;
  thiz->first = NULL;
}

void NQPrimitiveStorage_finalize(NQPrimitiveStorage* thiz)
{
  struct NQPrimitiveStorageEntry* entry = thiz->first;
  while (entry != NULL) {
    struct NQPrimitiveStorageEntry* curr = entry;
    entry = entry->next;
    NQFree(curr);
  }
}

static struct NQPrimitiveStorageEntry* NQPrimitiveStorage_getEntry(NQPrimitiveStorage* thiz, const char* name)
{
  struct NQPrimitiveStorageEntry* entry = thiz->first;
  while (entry != NULL) {
    if (!strcmp(entry->name, name))
      return entry;
    entry = entry->next;
  }
  return NULL;
}

size_t NQPrimitiveStorage_getUint32(NQPrimitiveStorage* thiz, const char* name, uint32_t* value)
{
  struct NQPrimitiveStorageEntry* entry = NQPrimitiveStorage_getEntry(thiz, name);

  if (entry != NULL) {
    if (entry->type == kNQUint32Type) {
      memcpy(value, &entry->dataUint32, sizeof(*value));
      return sizeof(*value);
    }
    if (entry->type == kNQPrimitiveStorageHandler) {
      return entry->dataHandler(entry->dataPointer, kNQUint32Type, (char*)value, sizeof(*value));
    }
  }

  if(thiz->parent != NULL) {
    return NQPrimitiveStorage_getUint32(thiz->parent, name, value);
  }

  return 0;
}

bool NQPrimitiveStorage_setUint32(NQPrimitiveStorage* thiz, const char* name, uint32_t value)
{
  size_t nlenz = strlen(name) + 1;

  struct NQPrimitiveStorageEntry* entry;
  entry = (struct NQPrimitiveStorageEntry*)NQMalloc(sizeof(*entry) + nlenz);
  if (entry == NULL)
    return false;

  entry->type = kNQUint32Type;

  char* ptr = (char*)thiz + sizeof(*entry);
  (void)memcpy(ptr, name, nlenz);
  entry->name = ptr;

  entry->dataUint32 = value;
  entry->dataLength = sizeof(value);

  entry->next = thiz->first;
  thiz->first = entry;

  return true;
}

size_t NQPrimitiveStorage_getString(NQPrimitiveStorage* thiz, const char* name, char* buffer, size_t size)
{
  struct NQPrimitiveStorageEntry* entry = NQPrimitiveStorage_getEntry(thiz, name);

  if (entry != NULL) {
    if (entry->type == kNQStringType) {
      size_t nlenz = NQGetMin(entry->dataLength + 1, size);
      memcpy(buffer, entry->dataString, nlenz);
      return entry->dataLength;
    }
    if (entry->type == kNQPrimitiveStorageHandler) {
      return entry->dataHandler(entry->dataPointer, kNQStringType, buffer, size);
    }
  }

  if(thiz->parent != NULL) {
    return NQPrimitiveStorage_getString(thiz->parent, name, buffer, size);
  }

  return 0;
}

bool NQPrimitiveStorage_setString(NQPrimitiveStorage* thiz, const char* name, const char* value)
{
  struct NQPrimitiveStorageEntry* entry;

  size_t nlenz = strlen(name) + 1;
  size_t vlenz = strlen(value) + 1;

  entry = (struct NQPrimitiveStorageEntry*)NQMalloc(sizeof(*entry) + nlenz + vlenz);
  if (entry == NULL)
    return false;

  entry->type = kNQStringType;

  char* ptr = (char*)thiz + sizeof(*entry);
  (void)memcpy(ptr, name, nlenz);
  entry->name = ptr;
  ptr += nlenz;

  (void)memcpy(ptr, value, vlenz);
  entry->dataString = ptr;
  entry->dataLength = vlenz - 1;

  entry->next = thiz->first;
  thiz->first = entry;

  return true;
}

bool NQPrimitiveStorage_setHandle(NQPrimitiveStorage* thiz, const char* name, NQPrimitiveStorageHandler* handler, void* userdata)
{
  size_t nlenz = strlen(name) + 1;

  struct NQPrimitiveStorageEntry* entry;
  entry = (struct NQPrimitiveStorageEntry*)NQMalloc(sizeof(*entry) + nlenz);
  if (entry == NULL)
    return false;

  entry->type = kNQUint32Type;

  char* ptr = (char*)thiz + sizeof(*entry);
  (void)memcpy(ptr, name, nlenz);
  entry->name = ptr;

  entry->dataHandler = handler;
  entry->dataPointer = userdata;

  entry->next = thiz->first;
  thiz->first = entry;

  return true;
}
