/*
 * MIT License
 *
 * Copyright (c) 2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_LEB128_H
#define _LIBNETQ_LEB128_H

#include <libnetq/Basic.h>
#include <libnetq/ConstExpr.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NQ_LEB128_BUFFER_SIZE (10)
typedef uint8_t NQLeb128Buffer[NQ_LEB128_BUFFER_SIZE];

NQ_EXPORT size_t NQLeb128EncodeInt8(void* buffer, size_t size, int8_t value);
NQ_EXPORT size_t NQLeb128EncodeInt16(void* buffer, size_t size, int16_t value);
NQ_EXPORT size_t NQLeb128EncodeInt32(void* buffer, size_t size, int32_t value);
NQ_EXPORT size_t NQLeb128EncodeInt64(void* buffer, size_t size, int64_t value);

NQ_EXPORT size_t NQLeb128EncodeUint8(void* buffer, size_t size, uint8_t value);
NQ_EXPORT size_t NQLeb128EncodeUint16(void* buffer, size_t size, uint16_t value);
NQ_EXPORT size_t NQLeb128EncodeUint32(void* buffer, size_t size, uint32_t value);
NQ_EXPORT size_t NQLeb128EncodeUint64(void* buffer, size_t size, uint64_t value);

NQ_EXPORT size_t NQLeb128DecodeInt8(const void* data, size_t size, int8_t* result);
NQ_EXPORT size_t NQLeb128DecodeInt16(const void* data, size_t size, int16_t* result);
NQ_EXPORT size_t NQLeb128DecodeInt32(const void* data, size_t size, int32_t* result);
NQ_EXPORT size_t NQLeb128DecodeInt64(const void* data, size_t size, int64_t* result);

NQ_EXPORT size_t NQLeb128DecodeUint8(const void* data, size_t size, uint8_t* result);
NQ_EXPORT size_t NQLeb128DecodeUint16(const void* data, size_t size, uint16_t* result);
NQ_EXPORT size_t NQLeb128DecodeUint32(const void* data, size_t size, uint32_t* result);
NQ_EXPORT size_t NQLeb128DecodeUint64(const void* data, size_t size, uint64_t* result);

struct NQSLEB128Ctx {
  int64_t value;
  uint32_t index;
  uint32_t size;
};

NQ_EXPORT void NQSLEB128Ctx_init(struct NQSLEB128Ctx*);
NQ_EXPORT bool NQSLEB128Ctx_add(struct NQSLEB128Ctx*, uint8_t byte);

struct NQULEB128Ctx {
  uint64_t value;
  uint32_t index;
  uint32_t size;
};

NQ_EXPORT void NQULEB128Ctx_init(struct NQULEB128Ctx*);
NQ_EXPORT bool NQULEB128Ctx_add(struct NQULEB128Ctx*, uint8_t byte);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_LEB128_H */
