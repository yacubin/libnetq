/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_STRING_STRINGSTORAGE_H
#define _LIBNETQ_STRING_STRINGSTORAGE_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQStringStorage NQStringStorage;
struct NQStringStorage {
  const char* characters;
  uint32_t length;
};

NQ_EXPORT void NQStringStorage_init(NQStringStorage*);
NQ_EXPORT void NQStringStorage_finalize(NQStringStorage*);

NQ_EXPORT bool NQStringStorage_setCharacters(NQStringStorage* thiz, const char* characters);
NQ_EXPORT bool NQStringStorage_setCharacters2(NQStringStorage*, const char* characters, size_t length);

#define NQStringStorage_characters(thiz) (thiz)->characters

static inline size_t NQStringStorage_length(const NQStringStorage* s)
{
  return s->length;
}

static inline bool NQStringStorage_isEmpty(const NQStringStorage* s)
{
  return s->length == 0;
}

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_STRING_STRINGSTORAGE_H */
