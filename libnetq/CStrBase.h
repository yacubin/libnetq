/*
 * MIT License
 *
 * Copyright (c) 2022-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CSTRBASE_H
#define _LIBNETQ_CSTRBASE_H

#include <string.h>
#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NQ_CSTR_LENGTH(s) (NQ_ARRAY_LENGTH(s)-1)

NQ_EXPORT const char* NQCStrEmpty();
NQ_EXPORT size_t NQCStrLen(const uint8_t* str);
NQ_EXPORT size_t NQCStrLen16(const uint16_t* str);
NQ_EXPORT char* NQCStrDuplicate(const char* str);
NQ_EXPORT char* NQCStrDuplicateWithLength(const char* str, size_t len);
NQ_EXPORT const char* NQCStrFindCStrn(const char* s1, const char* s2, size_t n);
NQ_EXPORT void NQCStrDestroy(const char* str);
NQ_EXPORT bool NQCStrStartsWith(const char* str, const char* search);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_CSTRBASE_H */
