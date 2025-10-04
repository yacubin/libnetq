/*
 * MIT License
 *
 * Copyright (c) 2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_STRINGLIST_H
#define _LIBNETQ_STRINGLIST_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQStringList NQStringList;

struct NQStringEntry {
  struct NQStringEntry* next;
  uint32_t length;
  char characters[1];
};

struct NQStringList {
  struct NQStringEntry* first;
  struct NQStringEntry* last;
};

static inline void NQStringList_init(NQStringList* thiz)
{
  thiz->first = NULL;
  thiz->last = NULL;
}

NQ_EXPORT void NQStringList_finalize(NQStringList*);
NQ_EXPORT bool NQStringList_append(NQStringList*, const char* characters);

#define NQStringList_first(thiz) (thiz)->first

static inline bool NQStringList_isEmpty(const NQStringList* thiz)
{
  return (thiz->first == NULL);
}

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_STRINGLIST_H */
