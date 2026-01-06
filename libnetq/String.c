/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#define NQ_CLASS_NAME "NQString"
#define NQ_LOG_TAG NQ_CLASS_NAME

#include "config.h"
#include "libnetq/String.h"

#include <libnetq/RefCount.h>
#include <libnetq/Sprintf.h>
#include <libnetq/Compiler.h>
#include <libnetq/Malloc.h>
#include <libnetq/Limits.h>
#include <libnetq/Assert.h>
#include <libnetq/Log.h>

struct NQString {
  NQRefCount refCount;
  uint32_t length;
  uint8_t flags;
  uint8_t bytes[1];
};

#define NQ_STRING_REF_COUNT_STATIC 0x80000000

static NQString s_emptyStringStatic = {
  { NQ_STRING_REF_COUNT_STATIC }, 0, 0, { '\0' },
};

static void NQString_init(NQString* s, size_t length)
{
  NQ_ASSERT(length < NQ_UINT32_MAX);
  NQRefCount_init(&s->refCount);
  s->length = (uint32_t)length;
  s->flags = 0;
}

#define NQString_alloc(length) ((NQString*)NQMalloc(sizeof(NQString) + length))
#define NQString_free(s) (NQFree((void*)s))

NQString* NQString_create(const char* characters)
{
  size_t length = strlen(characters);
  return NQString_createWithLength(characters, length);
}

NQString* NQString_createWithLength(const char* characters, size_t length)
{
  if (NQ_UINT32_MAX <= length) {
    NQ_LOGE("Not support large string");
    return NULL;
  }

  NQString* s = NQString_alloc(length);
  if (s == NULL)
    return NULL;

  NQString_init(s, length);
  memcpy(s->bytes, characters, length);
  s->bytes[length] = '\0';

  return s;
}

NQString* NQString_format(const char* format, ...)
{
  NQ_ASSERT(format);

  int ret;
  NQString* s;
  uint32_t length;
  va_list args;

  va_start(args, format);

#ifdef NQ_COMPILER_MSVC
  ret = _vscprintf(format, args);
#else
  char ch;
  ret = vsnprintf(&ch, 1, format, args);
  va_end(args);
  va_start(args, format);
#endif
  
  if (ret == 0 || ret < 0)
    s = &s_emptyStringStatic;
  else {
    length = (size_t)ret;
    s = NQString_alloc(length);
    if (s != NULL)
      vsnprintf((char*)s->bytes, length + 1, format, args);
  }

  va_end(args);

  return s;
}

NQString* NQString_retain(NQString* s)
{
  NQRefCount_ref(&s->refCount);
  return s;
}

void NQString_destroy(NQString* s)
{
  NQRefCount_unref(&s->refCount, NQString_free, s);
}

const char* NQString_characters(const NQString* s)
{
  return (const char*)s->bytes;
}

size_t NQString_length(const NQString* s)
{
  return s->length;
}

bool NQString_isEmpty(const NQString* s)
{
  return s->length == 0;
}
