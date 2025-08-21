/*
 * MIT License
 *
 * Copyright (c) 2022-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_KEYVAL_H
#define _LIBNETQ_KEYVAL_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQKeyVal NQKeyVal;
typedef struct NQKeyValIter NQKeyValIter;

NQ_EXPORT NQKeyVal* NQKeyVal_create();
NQ_EXPORT void NQKeyVal_destroy(NQKeyVal*);

NQ_EXPORT const char* NQKeyVal_get(NQKeyVal*, const char* key);
NQ_EXPORT bool NQKeyVal_set(NQKeyVal*, const char* key, const char* val);

NQ_EXPORT NQKeyValIter* NQKeyVal_begin(NQKeyVal*);
NQ_EXPORT NQKeyValIter* NQKeyValIter_next(NQKeyValIter*);
NQ_EXPORT const char* NQKeyValIter_key(NQKeyValIter*);
NQ_EXPORT const char* NQKeyValIter_val(NQKeyValIter*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_KEYVAL_H */
