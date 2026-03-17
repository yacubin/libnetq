/*
 * MIT License
 *
 * Copyright (c) 2023-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_WEB_URLHOST_H
#define _LIBNETQ_WEB_URLHOST_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQUrlHost NQUrlHost;

NQ_EXPORT NQUrlHost* NQUrlHost_create(const char* characters);
NQ_EXPORT void NQUrlHost_destroy(NQUrlHost*);

NQ_EXPORT const char* NQUrlHost_characters(const NQUrlHost*);
NQ_EXPORT uint16_t NQUrlHost_port(const NQUrlHost*);
NQ_EXPORT void NQUrlHost_setPort(NQUrlHost*, uint16_t port);
NQ_EXPORT bool NQUrlHost_hasPort(const NQUrlHost*);

NQ_EXPORT bool NQUrlHost_equal(const NQUrlHost*, const NQUrlHost* other);
NQ_EXPORT bool NQUrlHost_equalCharacters(const NQUrlHost*, const char* characters);
NQ_EXPORT bool NQUrlHost_isIPv4Address(const NQUrlHost*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_WEB_URLHOST_H */
