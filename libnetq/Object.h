/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_OBJECT_H
#define _LIBNETQ_OBJECT_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* NQObject;

void NQObject_destroy(NQObject* o);

int NQObject_type(const NQObject* o);
const char* NQObject_name(const NQObject* o);
unsigned NQObject_version(const NQObject* o);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_OBJECT_H */
