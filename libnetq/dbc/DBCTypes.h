/*
 * MIT License
 *
 * Copyright (c) 2025-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_DBC_DBCTYPES_H
#define _LIBNETQ_DBC_DBCTYPES_H

#include <libnetq/Basic.h>
#include <libnetq/dbc/DBCKeywords.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t NQDBCByteOrder;
enum {
  kNQDBCBigEndian = '0',
  kNQDBCLittleEndian = '1',
};

NQ_EXPORT const char* NQDBCByteOrder_toCString(NQDBCByteOrder byteOrder);

typedef uint8_t NQDBCSigValueType;
enum {
  kNQDBCSigValueInteger = 0,
  kNQDBCSigValueFloat = 1,
  kNQDBCSigValueDouble = 2,
  kNQDBCSigValueUnknown = 3,
};

NQ_EXPORT const char* NQDBCSigValueType_toCString(NQDBCSigValueType valueType);

typedef uint32_t NQDBCEnvId;

#define NQDBC_READ_FLAG (1 << 0)
#define NQDBC_WRITE_FLAG (1 << 1)
#define NQDBC_STRING_FLAG (1 << 15)

typedef uint16_t NQDBCEnvAccessType;

typedef uint8_t NQDBCEnvValueType;
enum {
  kNQDBCEnvValueInteger = 0,
  kNQDBCEnvValueFloat = 1,
  kNQDBCEnvValueString = 2,
};

typedef struct NQDBCEnvInfo NQDBCEnvInfo;
struct NQDBCEnvInfo {
  NQDBCEnvId id; /* obsolete */
  NQDBCEnvValueType valueType;
  NQDBCEnvAccessType accessType;

  double minimum;
  double maximum;
  double  initialValue;

  const char* unit;
};

typedef uint8_t NQDBCObjectType;
enum {
  kNQDBCObjectDocument,
  kNQDBCObjectNetNode,
  kNQDBCObjectMessage,
  kNQDBCObjectSignal,
  kNQDBCObjectEnvVar,
};

typedef uint8_t NQDBCAttrValueType;
enum {
  kNQDBCAttrValueInt,
  kNQDBCAttrValueHex,
  kNQDBCAttrValueFloat,
  kNQDBCAttrValueString,
  kNQDBCAttrValueEnum,
};

NQ_EXPORT const char* NQDBCAttrValueType_toCString(NQDBCAttrValueType);

struct NQDBCAttrValueInfo {
  NQDBCAttrValueType type;
  union {
    struct {
      int32_t first;
      int32_t second;
    } vInt;

    struct {
      int32_t first;
      int32_t second;
    } vHex;

    struct {
      double first;
      double second;
    } vFloat;

    struct {
      const char** data;
      size_t count;
    } vEnum;
  };
};

typedef uint8_t NQDBCVariantType;
enum {
  kNQDBCVariantInt = 0,
  kNQDBCVariantUint = 1,
  kNQDBCVariantFloat = 2,
  kNQDBCVariantString = 3,
};

typedef struct NQDBCVariant NQDBCVariant;
struct NQDBCVariant {
  NQDBCVariantType type;
  union {
    int64_t vInt;
    double vFloat;
    const char* vString;
  };
};

typedef struct NQDBCValDesc NQDBCValDesc;
struct NQDBCValDesc {
  uint32_t value;
  const char* description;
};

typedef struct NQDBCSigInfo NQDBCSigInfo;
struct NQDBCSigInfo {
  uint32_t sizeInBits;
  NQDBCByteOrder byteOrder;
  bool isUnsigned;
  double factor;
  double offset;
  double minimum;
  double maximum;
  const char* unit;
};

typedef struct NQDBCRange NQDBCRange;
struct NQDBCRange {
  uint32_t first;
  uint32_t second;
};

typedef struct NQDBCBitTiming NQDBCBitTiming;
struct NQDBCBitTiming {
  uint32_t baudrate;
  uint32_t btr1;
  uint32_t btr2;
};

typedef struct NQDBCMulSwValue NQDBCMulSwValue;
struct NQDBCMulSwValue {
  bool has;
  uint32_t value;
};

typedef struct NQDBCTarget NQDBCTarget;
struct NQDBCTarget {
  NQDBCObjectType objectType;

  union {
    struct {
      const char* name;
    } netNode;

    struct {
      uint32_t id;
    } message;

    struct {
      uint32_t messageId;
      const char* name;
    } signal;

    struct {
      const char* name;
    } envVar;
  };
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _LIBNETQ_DBC_DBCTYPES_H */
