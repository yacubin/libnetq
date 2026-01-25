/*
 * MIT License
 *
 * Copyright (c) 2023-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_JSON_JSONWRITER_H
#define _LIBNETQ_JSON_JSONWRITER_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef bool (NQJSONWriterCallback) (void* userdata, const char* characters, size_t size);

typedef struct NQJSONWriter NQJSONWriter;
struct NQJSONWriter {
  NQJSONWriterCallback* callback;
  void* userdata;
  size_t count;
  bool hasError;

  struct {
    uint8_t isObject : 1;
    uint8_t hasChild : 1;
    uint8_t hasKey : 1;
  } depth[255];

  uint16_t depthIndex;
};

NQ_EXPORT void NQJSONWriter_init(NQJSONWriter*, NQJSONWriterCallback* callback, void* userdata);
NQ_EXPORT void NQJSONWriter_init5(NQJSONWriter*, NQJSONWriterCallback* callback, void* userdata);
NQ_EXPORT void NQJSONWriter_finalize(NQJSONWriter*);

NQ_EXPORT bool NQJSONWriter_writeObjectBegin(NQJSONWriter*);
NQ_EXPORT bool NQJSONWriter_writeObjectEnd(NQJSONWriter*);

NQ_EXPORT bool NQJSONWriter_writeArrayBegin(NQJSONWriter*);
NQ_EXPORT bool NQJSONWriter_writeArrayEnd(NQJSONWriter*);

NQ_EXPORT bool NQJSONWriter_writeNull(NQJSONWriter*);
NQ_EXPORT bool NQJSONWriter_writeChar(NQJSONWriter*, char val);
NQ_EXPORT bool NQJSONWriter_writeString(NQJSONWriter*, const char* val);
NQ_EXPORT bool NQJSONWriter_writeString2(NQJSONWriter*, const char* val, size_t len);
NQ_EXPORT bool NQJSONWriter_writeInt64(NQJSONWriter*, int64_t val);
NQ_EXPORT bool NQJSONWriter_writeInt32(NQJSONWriter*, int32_t val);
NQ_EXPORT bool NQJSONWriter_writeInt16(NQJSONWriter*, int16_t val);
NQ_EXPORT bool NQJSONWriter_writeInt8(NQJSONWriter*, int8_t val);
NQ_EXPORT bool NQJSONWriter_writeUint64(NQJSONWriter*, uint64_t val);
NQ_EXPORT bool NQJSONWriter_writeUint32(NQJSONWriter*, uint32_t val);
NQ_EXPORT bool NQJSONWriter_writeUint16(NQJSONWriter*, uint16_t val);
NQ_EXPORT bool NQJSONWriter_writeUint8(NQJSONWriter*, uint8_t val);
NQ_EXPORT bool NQJSONWriter_writeFloat(NQJSONWriter*, float val);
NQ_EXPORT bool NQJSONWriter_writeDouble(NQJSONWriter*, double val);
NQ_EXPORT bool NQJSONWriter_writeBool(NQJSONWriter*, bool val);

NQ_EXPORT bool NQJSONWriter_writeKey(NQJSONWriter*, const char* key);
NQ_EXPORT bool NQJSONWriter_writeKeyObjectBegin(NQJSONWriter*, const char* key);
NQ_EXPORT bool NQJSONWriter_writeKeyArrayBegin(NQJSONWriter*, const char* key);
NQ_EXPORT bool NQJSONWriter_writeKeyNull(NQJSONWriter*, const char* key);
NQ_EXPORT bool NQJSONWriter_writeKeyChar(NQJSONWriter*, const char* key, char val);
NQ_EXPORT bool NQJSONWriter_writeKeyString(NQJSONWriter*, const char* key, const char* val);
NQ_EXPORT bool NQJSONWriter_writeKeyString2(NQJSONWriter*, const char* key, const char* val, size_t len);
NQ_EXPORT bool NQJSONWriter_writeKeyInt64(NQJSONWriter*, const char* key, int64_t val);
NQ_EXPORT bool NQJSONWriter_writeKeyInt32(NQJSONWriter*, const char* key, int32_t val);
NQ_EXPORT bool NQJSONWriter_writeKeyInt16(NQJSONWriter*, const char* key, int16_t val);
NQ_EXPORT bool NQJSONWriter_writeKeyInt8(NQJSONWriter*, const char* key, int8_t val);
NQ_EXPORT bool NQJSONWriter_writeKeyUint64(NQJSONWriter*, const char* key, uint64_t val);
NQ_EXPORT bool NQJSONWriter_writeKeyUint32(NQJSONWriter*, const char* key, uint32_t val);
NQ_EXPORT bool NQJSONWriter_writeKeyUint16(NQJSONWriter*, const char* key, uint16_t val);
NQ_EXPORT bool NQJSONWriter_writeKeyUint8(NQJSONWriter*, const char* key, uint8_t val);
NQ_EXPORT bool NQJSONWriter_writeKeyFloat(NQJSONWriter*, const char* key, float val);
NQ_EXPORT bool NQJSONWriter_writeKeyDouble(NQJSONWriter*, const char* key, double val);
NQ_EXPORT bool NQJSONWriter_writeKeyBool(NQJSONWriter*, const char* key, bool val);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_JSON_JSONWRITER_H */
