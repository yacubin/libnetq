/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/StringPrint.h"

#include <libnetq/CStrBase.h>
#include <libnetq/Sprintf.h>
#include <libnetq/Malloc.h>
#include <libnetq/Limits.h>
#include <libnetq/Math.h>
#include <libnetq/Assert.h>

void NQStringPrint_init(NQStringPrint* thiz)
{
  thiz->characters = thiz->buffer;
  thiz->length = 0;
  thiz->capacity = sizeof(thiz->buffer);
  thiz->characters[0] = '\0';
}

void NQStringPrint_finalize(NQStringPrint* thiz)
{
  if (thiz->characters != thiz->buffer)
    NQFree(thiz->characters);
}

static bool NQStringPrint_resize(NQStringPrint* thiz, size_t newSize)
{
  NQ_ASSERT(newSize > thiz->capacity);
  NQ_ASSERT(newSize > sizeof(thiz->buffer));

  thiz->capacity = newSize << 1;

  char* newBuffer = (char*)(NQMalloc(thiz->capacity));
  if (newBuffer == NULL)
    return false;

  memcpy(newBuffer, thiz->characters, thiz->length + 1);
  if (thiz->characters != thiz->buffer)
    NQFree(thiz->characters);
  thiz->characters = newBuffer;

  return true;
}

void NQStringPrint_printf(NQStringPrint* thiz, const char* format, ...)
{
  va_list list;
  va_start(list, format);
  NQStringPrint_vprintf(thiz, format, list);
  va_end(list);
}

int NQStringPrint_vprintf(NQStringPrint* thiz, const char* format, va_list list)
{
  NQ_ASSERT(thiz->length < thiz->capacity);
  NQ_ASSERT(!thiz->characters[thiz->length]);

  va_list listCopy;
  va_copy(listCopy, list);

  int n = vsnprintf(thiz->characters + thiz->length, thiz->capacity - thiz->length, format, listCopy);

  va_end(listCopy);

  int k = n + 1;

  if (thiz->length + k <= thiz->capacity) {
    thiz->length += n;
    return n;
  }

  if (!NQStringPrint_resize(thiz, thiz->length + k))
    return -1;

  int ret = vsnprintf(thiz->characters + thiz->length, thiz->capacity - thiz->length, format, list);
  
  NQ_ASSERT(thiz->length + ret + 1 <= thiz->capacity);

  thiz->length += ret;

  NQ_ASSERT(thiz->length < thiz->capacity);
  NQ_ASSERT(!thiz->characters[thiz->length]);

  return ret;
}

int NQStringPrint_write(NQStringPrint* thiz, const char* characters, size_t length)
{
  length = NQGetMin(NQ_INT32_MAX, length);

  int k = (int)length + 1;
  if (thiz->length + k > thiz->capacity) {
    if (!NQStringPrint_resize(thiz, thiz->length + k))
      return -1;
  }

  memcpy(thiz->characters + thiz->length, characters, length);
  thiz->length += length;
  thiz->characters[thiz->length] = '\0';

  return (int)length;
}

void NQStringPrint_reset(NQStringPrint* thiz)
{
  thiz->length = 0;
  thiz->characters[0] = '\0';
}
