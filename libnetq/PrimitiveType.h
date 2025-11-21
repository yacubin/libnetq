/*
 * MIT License
 *
 * Copyright (c) 2023-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_PRIMITIVETYPE_H
#define _LIBNETQ_PRIMITIVETYPE_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NQ_BOOL_STRING "bool"
#define NQ_CHAR_STRING "char"
#define NQ_INT8_STRING "int8"
#define NQ_INT16_STRING "int16"
#define NQ_INT32_STRING "int32"
#define NQ_INT64_STRING "int64"
#define NQ_UINT8_STRING "uint8"
#define NQ_UINT16_STRING "uint16"
#define NQ_UINT32_STRING "uint32"
#define NQ_UINT64_STRING "uint64"
#define NQ_FLOAT_STRING "float"
#define NQ_DOUBLE_STRING "double"
#define NQ_STRING_STRING "string"

#define NQ_NULL_STRING "null"
#define NQ_FALSE_STRING "false"
#define NQ_TRUE_STRING "true"
#define NQ_OFF_STRING "off"
#define NQ_ON_STRING "on"
#define NQ_ZERO_STRING "0"
#define NQ_ONE_STRING "1"

NQ_EXPORT bool NQIsFalse(const char* s);
NQ_EXPORT bool NQIsTrue(const char* s);
NQ_EXPORT bool NQIsOff(const char* s);
NQ_EXPORT bool NQIsOn(const char* s);
NQ_EXPORT bool NQIsEnabled(const char* s);
NQ_EXPORT bool NQIsDisabled(const char* s);

typedef enum NQPrimitiveType {
  kNQBoolType,
  kNQCharType,
  kNQInt8Type,
  kNQInt16Type,
  kNQInt32Type,
  kNQInt64Type,
  kNQUint8Type,
  kNQUint16Type,
  kNQUint32Type,
  kNQUint64Type,
  kNQFloatType,
  kNQDoubleType,
  kNQStringType,
} NQPrimitiveType;

NQ_EXPORT const char* NQPrimitiveTypeToSTRINGing(NQPrimitiveType type);
NQ_EXPORT bool NQParsePrimitiveType(const char* str, NQPrimitiveType* result);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_PRIMITIVETYPE_H */
