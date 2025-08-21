/*
 * MIT License
 *
 * Copyright (c) 2021-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_OBJECTCLASS_H
#define _LIBNETQ_OBJECTCLASS_H

#include <libnetq/Object.h>
#include <libnetq/ObjectType.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*NQObjectReleaseCallback) (NQObject* o);

typedef struct NQObjectClass {
  int type;
  const char* name;
  unsigned version;

  NQObjectReleaseCallback release;

} NQObjectClass;

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_OBJECTCLASS_H */
