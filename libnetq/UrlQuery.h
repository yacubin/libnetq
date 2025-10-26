/*
 * MIT License
 *
 * Copyright (c) 2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_URLQUERY_H
#define _LIBNETQ_URLQUERY_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQUrlQuery NQUrlQuery;

NQ_EXPORT NQUrlQuery* NQUrlQuery_create(const char* query);
NQ_EXPORT void NQUrlQuery_destroy(NQUrlQuery*);
NQ_EXPORT size_t NQUrlQuery_count(const NQUrlQuery*);
NQ_EXPORT const char* NQUrlQuery_nameAt(const NQUrlQuery*, size_t index);
NQ_EXPORT const char* NQUrlQuery_valueAt(const NQUrlQuery*, size_t index);
NQ_EXPORT const char* NQUrlQuery_value(const NQUrlQuery*, const char* name);
NQ_EXPORT const char* NQUrlQuery_query(const NQUrlQuery*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_URLQUERY_H */
