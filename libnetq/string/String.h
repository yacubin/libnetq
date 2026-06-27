/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_STRING_STRING_H
#define _LIBNETQ_STRING_STRING_H

#include <libnetq/Basic.h>

#if defined(NQ_OS_KERNEL)
#include <linux/string.h>
#define NQ_HAVE_ARCH_STRLEN 1
#define NQ_HAVE_ARCH_STRRCHR 1
#elif defined(NQ_OS_WINDOWS) || defined(NQ_OS_UNIX) || defined(NQCONFIG_USE_STRING_H)
#include <string.h>
#define NQ_HAVE_ARCH_STRLEN 1
#define NQ_HAVE_ARCH_STRRCHR 1
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(NQ_COMPILER_MSVC)
#define NQStrcasecmp _stricmp
#define NQStrncasecmp _strnicmp
#else
#define NQStrcasecmp strcasecmp
#define NQStrncasecmp strncasecmp
#endif

#ifndef NQ_HAVE_ARCH_STRLEN
NQ_EXPORT size_t NQStrlen(const char* s);
#else
#define NQStrlen strlen
#endif

#ifndef NQ_HAVE_ARCH_STRRCHR
NQ_EXPORT char* NQStrrchr(const char* s, int c);
#else
#define NQStrrchr strrchr
#endif

#define NQStrcmp strcmp
#define NQStrncmp strncmp

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_STRING_STRING_H */
