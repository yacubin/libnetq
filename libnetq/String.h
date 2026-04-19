/*
 * MIT License
 *
 * Copyright (c) 2020-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_STRING_H
#define _LIBNETQ_STRING_H

#include <libnetq/CStrBase.h>
#include <libnetq/RefCount.h>
#include <libnetq/string/StringArray.h>
#include <libnetq/string/StringStorage.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQString NQString;
struct NQString {
  NQRefCount refCount;
  uint32_t length;
  uint8_t flags;
  char characters[1];
};

NQ_EXPORT NQString* NQString_alloc(size_t length);
NQ_EXPORT NQString* NQString_create(const char* characters);
NQ_EXPORT NQString* NQString_create2(const char* characters, size_t length);
NQ_EXPORT NQString* NQString_format(const char* format, ...) NQ_ATTRIBUTE_PRINTF(1, 2);
NQ_EXPORT NQString* NQString_fromFile(const char* filename);
NQ_EXPORT NQString* NQString_retain(NQString* s);
NQ_EXPORT void NQString_release(NQString* s);

#define NQString_characters(thiz) (thiz)->characters

static inline size_t NQString_length(const NQString* s)
{
  return s->length;
}

static inline bool NQString_isEmpty(const NQString* s)
{
  return s->length == 0;
}

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_STRING_H */
