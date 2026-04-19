/*
 * MIT License
 *
 * Copyright (c) 2025-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/dbc/DBCTypes.h"

#include <libnetq/String.h>
#include <libnetq/Assert.h>

const char* NQDBCByteOrder_toCString(NQDBCByteOrder byteOrder)
{
  switch (byteOrder) {
  case kNQDBCBigEndian:
    return "BIG_ENDIAN";
  case kNQDBCLittleEndian:
    return "LITTLE_ENDIAN";
  }

  NQ_ASSERT_NOT_REACHED();
  return NULL;
}

const char* NQDBCSigValueType_toCString(NQDBCSigValueType valueType)
{
  switch (valueType) {
  case kNQDBCSigValueInteger:
    return "INTEGER";
  case kNQDBCSigValueFloat:
    return "FLOAT";
  case kNQDBCSigValueDouble:
    return "DOUBLE";
  case kNQDBCSigValueUnknown:
    return "UNKNOWN";
  }

  NQ_ASSERT_NOT_REACHED();
  return NULL;
}

const char* NQDBCAttrValueType_toCString(NQDBCAttrValueType valueType)
{
  switch (valueType) {
  case kNQDBCAttrValueInt:
    return "INT";
  case kNQDBCAttrValueHex:
    return "HEX";
  case kNQDBCAttrValueFloat:
    return "FLOAT";
  case kNQDBCAttrValueString:
    return "STRING";
  case kNQDBCAttrValueEnum:
    return "ENUM";
  }

  NQ_ASSERT_NOT_REACHED();
  return NULL;
}
