/*
 * MIT License
 *
 * Copyright (c) 2021-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/Object.h"

#include <libnetq/ObjectClass.h>
#include <libnetq/Malloc.h>
#include <libnetq/Assert.h>

typedef struct NQObjectAlias {
  const NQObjectClass* class;
} NQObjectAlias;

void NQObject_destroy(NQObject* o)
{
  NQObjectAlias* object = (NQObjectAlias*)o;
  if (object->class->release)
    object->class->release(o);
}

int NQObject_type(const NQObject* o)
{
  NQObjectAlias* object = (NQObjectAlias*)o;
  if (object->class != NULL)
    return object->class->type;
  return 0;
}

const char* NQObject_name(const NQObject* o)
{
  NQObjectAlias* object = (NQObjectAlias*)o;
  if (object->class != NULL)
    return object->class->name;
  return "";
}

unsigned NQObject_version(const NQObject* o)
{
  NQObjectAlias* object = (NQObjectAlias*)o;
  if (object->class != NULL)
    return object->class->version;
  return 0;
}
