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

NQ_EXPORT const char* NQPrimitiveTypeToCString(NQPrimitiveType type);
NQ_EXPORT bool NQParsePrimitiveType(const char* str, NQPrimitiveType* result);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_PRIMITIVETYPE_H */
