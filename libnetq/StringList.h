/*
 * MIT License
 *
 * Copyright (c) 2025-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_STRINGLIST_H
#define _LIBNETQ_STRINGLIST_H

#include <libnetq/Basic.h>
#include <libnetq/List.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQStringList NQStringList;
typedef struct NQStringListIter NQStringListIter;

struct NQStringList {
  NQListHead impl;
  size_t size;
};

static inline void NQStringList_init(NQStringList* thiz)
{
  NQListHead_init(&thiz->impl);
  thiz->size = 0;
}

NQ_EXPORT void NQStringList_finalize(NQStringList*);
NQ_EXPORT bool NQStringList_append(NQStringList*, const char* characters);
NQ_EXPORT bool NQStringList_append2(NQStringList*, const char* characters, size_t length);
NQ_EXPORT bool NQStringList_split(NQStringList*, const char* str, char separator);

NQ_EXPORT const NQStringListIter* NQStringList_firstIter(const NQStringList*);
NQ_EXPORT const NQStringListIter* NQStringList_nextIter(const NQStringList*, const NQStringListIter* iter);

NQ_EXPORT uint32_t NQStringListIter_length(const NQStringListIter*);
NQ_EXPORT const char* NQStringListIter_characters(const NQStringListIter*);

static inline bool NQStringList_isEmpty(const NQStringList* thiz)
{
  return NQListHead_isEmpty(&thiz->impl);
}

static inline size_t NQStringList_size(const NQStringList* thiz)
{
  return thiz->size;
}

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_STRINGLIST_H */
