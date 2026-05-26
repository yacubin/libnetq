/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_STRING_STRINGVEC_H
#define _LIBNETQ_STRING_STRINGVEC_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQStringVec NQStringVec;
struct NQStringVec {
  const char* characters;
  uint32_t length;
};

NQ_EXPORT void NQStringVec_init(NQStringVec*);
NQ_EXPORT void NQStringVec_finalize(NQStringVec*);

NQ_EXPORT bool NQStringVec_setCharacters(NQStringVec* thiz, const char* characters);
NQ_EXPORT bool NQStringVec_setCharacters2(NQStringVec*, const char* characters, size_t length);

#define NQStringVec_characters(thiz) (thiz)->characters

static inline size_t NQStringVec_length(const NQStringVec* s)
{
  return s->length;
}

static inline bool NQStringVec_isEmpty(const NQStringVec* s)
{
  return s->length == 0;
}

NQ_EXPORT bool NQStringVec_isEqual(const NQStringVec*, const char* str);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_STRING_STRINGVEC_H */
