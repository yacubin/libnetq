/*
 * MIT License
 *
 * Copyright (c) 2023-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/PrimitiveType.h"

#include <libnetq/CStrBase.h>

#define NQ_BOOL_CSTR "bool"
#define NQ_CHAR_CSTR "char"
#define NQ_INT8_CSTR "int8"
#define NQ_INT16_CSTR "int16"
#define NQ_INT32_CSTR "int32"
#define NQ_INT64_CSTR "int64"
#define NQ_UINT8_CSTR "uint8"
#define NQ_UINT16_CSTR "uint16"
#define NQ_UINT32_CSTR "uint32"
#define NQ_UINT64_CSTR "uint64"
#define NQ_FLOAT_CSTR "float"
#define NQ_DOUBLE_CSTR "double"
#define NQ_STRING_CSTR "string"

const char* NQPrimitiveTypeToCString(NQPrimitiveType type)
{
  switch (type) {
  case kNQBoolType:
    return NQ_BOOL_CSTR;
  case kNQCharType:
    return NQ_CHAR_CSTR;
  case kNQInt8Type:
    return NQ_INT8_CSTR;
  case kNQInt16Type:
    return NQ_INT16_CSTR;
  case kNQInt32Type:
    return NQ_INT32_CSTR;
  case kNQInt64Type:
    return NQ_INT64_CSTR;
  case kNQUint8Type:
    return NQ_UINT8_CSTR;
  case kNQUint16Type:
    return NQ_UINT16_CSTR;
  case kNQUint32Type:
    return NQ_UINT32_CSTR;
  case kNQUint64Type:
    return NQ_UINT64_CSTR;
  case kNQFloatType:
    return NQ_FLOAT_CSTR;
  case kNQDoubleType:
    return NQ_DOUBLE_CSTR;
  case kNQStringType:
    return NQ_STRING_CSTR;
  }

  return NULL;
}

bool NQParsePrimitiveType(const char* str, NQPrimitiveType* result)
{
  NQPrimitiveType type;

  if (!strcmp(str, NQ_BOOL_CSTR))
    type = kNQBoolType;
  else if (!strcmp(str, NQ_CHAR_CSTR))
    type = kNQCharType;
  else if (!strcmp(str, NQ_INT8_CSTR))
    type = kNQInt8Type;
  else if (!strcmp(str, NQ_INT16_CSTR))
    type = kNQInt16Type;
  else if (!strcmp(str, NQ_INT32_CSTR))
    type = kNQInt32Type;
  else if (!strcmp(str, NQ_INT64_CSTR))
    type = kNQInt64Type;
  else if (!strcmp(str, NQ_UINT8_CSTR))
    type = kNQUint8Type;
  else if (!strcmp(str, NQ_UINT16_CSTR))
    type = kNQUint16Type;
  else if (!strcmp(str, NQ_UINT32_CSTR))
    type = kNQUint32Type;
  else if (!strcmp(str, NQ_UINT64_CSTR))
    type = kNQUint64Type;
  else if (!strcmp(str, NQ_FLOAT_CSTR))
    type = kNQFloatType;
  else if (!strcmp(str, NQ_DOUBLE_CSTR))
    type = kNQDoubleType;
  else if (!strcmp(str, NQ_STRING_CSTR))
    type = kNQStringType;
  else
    return false;

  if (result)
    *result = type;

  return true;
}
