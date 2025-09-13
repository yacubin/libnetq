/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#define NQ_CLASS_NAME "NQUint8Array"
#define NQ_LOG_TAG NQ_CLASS_NAME

#include "config.h"
#include "libnetq/Array.h"

#include <libnetq/CStrBase.h>
#include <libnetq/ObjectClass.h>
#include <libnetq/Malloc.h>
#include <libnetq/Limits.h>

extern const NQObjectClass __NQUint8ArrayClass;

struct NQUint8Array {
  const NQObjectClass* class;
  uint32_t size;
  uint8_t data[1];
};

static NQUint8Array s_zeroUint8Array = { &__NQUint8ArrayClass, 0, {0} };

static void NQUint8Array_initWithSize(NQUint8Array* array, uint32_t size)
{
  array->class = &__NQUint8ArrayClass;
  array->size = size;
}

static void NQUint8Array_init(NQUint8Array* array, const uint8_t* data, uint32_t size)
{
  NQUint8Array_initWithSize(array, size);
  memcpy(array->data, data, size);
}

NQUint8Array* NQUint8Array_alloc(size_t size)
{
  if (NQ_UINT32_MAX < size)
    return NULL;

  if (size == 0)
    return &s_zeroUint8Array;

  NQUint8Array* array = (NQUint8Array*)NQMalloc(sizeof(NQUint8Array) + size - 1);
  if (array == NULL)
    return NULL;

  NQUint8Array_initWithSize(array, (uint32_t)size);
  return array;
}

NQUint8Array* NQUint8Array_create(const uint8_t* data, size_t size)
{
  NQUint8Array* array = NQUint8Array_alloc(size);
  if (array == NULL)
    return NULL;

  memcpy(array->data, data, size);
  return array;
}

void NQUint8Array_destroy(NQUint8Array* array)
{
  if (array == &s_zeroUint8Array)
    return;
  
  NQFree((void*)array);
}

NQUint8Array* NQUint8Array_copy(const NQUint8Array* array)
{
  if (array == &s_zeroUint8Array)
    return &s_zeroUint8Array;

  NQUint8Array* newArray = (NQUint8Array*)NQMalloc(sizeof(NQUint8Array) + array->size - 1);
  if (newArray == NULL)
    return NULL;

  NQUint8Array_init(newArray, array->data, array->size);
  return newArray;
}

uint8_t* NQUint8Array_data(NQUint8Array* array)
{
  return array->data;
}

const uint8_t* NQUint8Array_cdata(const NQUint8Array* array)
{
  return array->data;
}

size_t NQUint8Array_size(const NQUint8Array* array)
{
  return array->size;
}

const NQObjectClass __NQUint8ArrayClass = {
  NQUint8ArrayObjectType,
  NQ_CLASS_NAME,
  NQ_VERSION_CODE,
  (NQObjectReleaseCallback)NQUint8Array_destroy,
};
