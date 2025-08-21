/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_STRING_H
#define _LIBNETQ_STRING_H

#include <string.h>

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NQ_NIL '\0'

#ifdef NQ_COMPILER_MSVC
#define nq_strncasecmp _strnicmp
#define nq_strnicmp _strnicmp
#define nq_stricmp _stricmp
#endif

#ifdef NQ_COMPILER_GCC
#define nq_strncasecmp strncasecmp
#define nq_strnicmp strncasecmp
#define nq_stricmp strcasecmp
#endif

typedef struct NQString NQString;

NQ_EXPORT NQString* NQString_create(const char* characters);
NQ_EXPORT NQString* NQString_createWithLength(const char* characters, size_t length);
NQ_EXPORT NQString* NQString_format(const char* format, ...) NQ_ATTRIBUTE_PRINTF(1, 2);
NQ_EXPORT NQString* NQString_retain(NQString* s);
NQ_EXPORT void NQString_destroy(NQString* s);

NQ_EXPORT const char* NQString_characters(const NQString* s);
NQ_EXPORT size_t NQString_length(const NQString* s);
NQ_EXPORT bool NQString_isEmpty(const NQString* s);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_STRING_H */
