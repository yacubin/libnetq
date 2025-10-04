/*
 * MIT License
 *
 * Copyright (c) 2023-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/JSONWriter.h"

#include <libnetq/CType.h>
#include <libnetq/CStrBase.h>
#include <libnetq/Sprintf.h>
#include <libnetq/Assert.h>
#include <libnetq/CStrBase.h>

static const char kObjectBegin = '{';
static const char kObjectEnd = '}';
static const char kArrayBegin = '[';
static const char kArrayEnd = ']';
static const char kStringBegin = '\"';
static const char kStringEnd = '\"';
static const char kNullStr[] = "null";
static const char kTrueStr[] = "true";
static const char kFalseStr[] = "false";
static const char kCommaStr = ',';
static const char kColonStr = ':';

#define kNoLength ((size_t)(~0))

void NQJSONWriter_init(NQJSONWriter* thiz, NQJSONWriterCallback* callback, void* userdata)
{
  thiz->callback = callback;
  thiz->userdata = userdata;
  thiz->count = 0;
  thiz->depthIndex = 0;
  thiz->hasError = false;
}

void NQJSONWriter_init5(NQJSONWriter* thiz, NQJSONWriterCallback* callback, void* userdata)
{
  thiz->callback = callback;
  thiz->userdata = userdata;
  thiz->count = 0;
  thiz->depthIndex = 0;
  thiz->hasError = false;
}

void NQJSONWriter_finalize(NQJSONWriter* thiz)
{
  NQ_ASSERT(!thiz->hasError);
}

static inline bool writeImpl(NQJSONWriter* thiz, const char* characters, size_t length)
{
  return (length != 0) ? thiz->callback(thiz->userdata, characters, length) : true;
}

static inline bool writeHex(NQJSONWriter* thiz, uint16_t code)
{
  char buf[8];
  int length = sprintf(buf, "\\u%04u", code);
  return thiz->callback(thiz->userdata, buf, (size_t)length);
}

static inline char num2ascii(uint8_t b)
{
  return ((b < 10) ? '0' : 'A' - 10) + b;
}

static inline bool writeEscape(NQJSONWriter* thiz, const char* characters, size_t length)
{
  // TODO: UTF8 and escape all bad symbols
  char buf[6] = { '\\' };
  size_t bufLen;

  const char* start = characters;
  const char* end = (length != kNoLength) ? characters + length : NULL;

  for (;;) {
    if (end == NULL) {
      if (*characters == '\0')
        break;
    }
    else if (end <= characters) {
      break;
    }

    char ch = *characters;

    switch (ch) {
    case '"':
      buf[1] = '"'; // Double quote
      bufLen = 2;
      break;
    case '\\':
      buf[1] = '\\'; // Backslash
      bufLen = 2;
      break;
#if 0
    case '/':
      buf[1] = '/'; // Solidus
      bufLen = 2;
      break;
#endif
    case '\b':
      buf[1] = 'b'; // Backspace
      bufLen = 2;
      break;
    case '\f':
      buf[1] = 'f'; // Form feed
      bufLen = 2;
      break;
    case '\n':
      buf[1] = 'n'; // Newline
      bufLen = 2;
      break;
    case '\r':
      buf[1] = 'r'; // Carriage return
      bufLen = 2;
      break;
    case '\t':
      buf[1] = 't'; // Tab
      bufLen = 2;
      break;
    default:
      if (NQIsPrintable(ch)) {
        characters++;
        continue;
      }
      buf[1] = 'u';
      buf[2] = '0';
      buf[3] = '0';
      buf[4] = num2ascii((ch & 0xf0) >> 4);
      buf[5] = num2ascii(ch & 0x0f);
      bufLen = 6;
      break;
    }

    if (!writeImpl(thiz, start, characters - start))
      return false;

    if (!writeImpl(thiz, buf, bufLen))
      return false;

    start = ++characters;
  }

  return writeImpl(thiz, start, characters - start);
}

static inline bool writeKeyDelimiter(NQJSONWriter* thiz)
{
  if (thiz->depthIndex == 0)
    return false;

  if (!thiz->depth[thiz->depthIndex].isObject)
    return false;

  if (thiz->depth[thiz->depthIndex].hasKey)
    return false;

  if (thiz->depth[thiz->depthIndex].hasChild) {
    if (!writeImpl(thiz, &kCommaStr, sizeof(kCommaStr)))
      return false;
  }

  thiz->depth[thiz->depthIndex].hasKey = true;
  return true;
}

static inline bool writeValDelimiter(NQJSONWriter* thiz)
{
  if (thiz->depthIndex == 0)
    return thiz->count == 0;

  if(thiz->depth[thiz->depthIndex].isObject) {
    if (!thiz->depth[thiz->depthIndex].hasKey)
      return false;
    if (!writeImpl(thiz, &kColonStr, sizeof(kColonStr)))
      return false;
    thiz->depth[thiz->depthIndex].hasKey = false;
  }
  else if (thiz->depth[thiz->depthIndex].hasChild) {
    if (!writeImpl(thiz, &kCommaStr, sizeof(kCommaStr)))
      return false;
  }

  thiz->depth[thiz->depthIndex].hasChild = true;
  return true;
}

static inline bool isLastDepth(const NQJSONWriter* thiz)
{
  return thiz->depthIndex >= (NQ_ARRAY_LENGTH(thiz->depth) - 1);
}

static inline bool writeKey(NQJSONWriter* thiz, const char* characters, size_t length)
{
  if (!writeKeyDelimiter(thiz))
    return false;

  if (!writeImpl(thiz, &kStringBegin, sizeof(kStringBegin)))
      return false;

  if (!writeEscape(thiz, characters, length))
      return false;

  if (!writeImpl(thiz, &kStringEnd, sizeof(kStringEnd)))
      return false;

  return true;
}

static inline bool writeString(NQJSONWriter* thiz, const char* characters, size_t length)
{
  if (!writeValDelimiter(thiz))
    return false;

  if (!writeImpl(thiz, &kStringBegin, sizeof(kStringBegin)))
      return false;

  if (!writeEscape(thiz, characters, length))
      return false;

  if (!writeImpl(thiz, &kStringEnd, sizeof(kStringEnd)))
      return false;

  thiz->count++;
  return true;
}

static inline bool writeObjectBegin(NQJSONWriter* thiz)
{
  if (isLastDepth(thiz))
      return false;

  if (!writeValDelimiter(thiz))
    return false;

  if (!writeImpl(thiz, &kObjectBegin, sizeof(kObjectBegin)))
    return false;

  thiz->depthIndex++;
  thiz->depth[thiz->depthIndex].hasChild = false;
  thiz->depth[thiz->depthIndex].isObject = true;
  thiz->depth[thiz->depthIndex].hasKey = false;
  return true;
}

static inline bool writeObjectEnd(NQJSONWriter* thiz)
{
  if (!thiz->depth[thiz->depthIndex].isObject)
    return false;

  if (!writeImpl(thiz, &kObjectEnd, sizeof(kObjectEnd)))
    return false;

  thiz->depthIndex--;
  thiz->count++;
  return true;
}

static inline bool writeArrayBegin(NQJSONWriter* thiz)
{
  if (isLastDepth(thiz))
      return false;

  if (!writeValDelimiter(thiz))
    return false;

  if (!writeImpl(thiz, &kArrayBegin, sizeof(kArrayBegin)))
    return false;

  thiz->depthIndex++;
  thiz->depth[thiz->depthIndex].hasChild = false;
  thiz->depth[thiz->depthIndex].isObject = false;
  thiz->depth[thiz->depthIndex].hasKey = false;
  return true;
}

static inline bool writeArrayEnd(NQJSONWriter* thiz)
{
  if (thiz->depth[thiz->depthIndex].isObject)
    return false;

  if (!writeImpl(thiz, &kArrayEnd, sizeof(kArrayEnd)))
    return false;

  thiz->depthIndex--;
  thiz->count++;
  return true;
}

static inline bool writeInt64(NQJSONWriter* thiz, int64_t val)
{
  if (!writeValDelimiter(thiz))
    return false;

  char buf[32];
  int length = sprintf(buf, "%lli", (long long int)val);
  if (length < 0 || length >= sizeof(buf))
    return false;

  if (!writeImpl(thiz, buf, length))
    return false;

  thiz->count++;
  return true;
}

static inline bool writeUint64(NQJSONWriter* thiz, uint64_t val)
{
  if (!writeValDelimiter(thiz))
    return false;

  char buf[32];
  int length = sprintf(buf, "%llu", (long long unsigned)val);
  if (length < 0 || length >= sizeof(buf))
    return false;

  if (!writeImpl(thiz, buf, length))
    return false;

  thiz->count++;
  return true;
}

static inline bool writeDouble(NQJSONWriter* thiz, double val)
{
#ifdef NQ_SYS_LINUX
  NQ_ASSERT_NOT_REACHED();
  return false;
#else
  if (!writeValDelimiter(thiz))
    return false;

  char buf[32];
  int length = sprintf(buf, "%f", val);
  if (length < 0 || length >= sizeof(buf))
    return false;

  if (!writeImpl(thiz, buf, length))
    return false;

  thiz->count++;
  return true;
#endif
}

static inline bool writeNull(NQJSONWriter* thiz)
{
  if (!writeValDelimiter(thiz))
    return false;

  if (!writeImpl(thiz, kNullStr, NQ_CSTR_LENGTH(kNullStr)))
    return false;

  thiz->count++;
  return true;
}

static inline bool writeBool(NQJSONWriter* thiz, bool val)
{
  if (!writeValDelimiter(thiz))
    return false;

  if (val) {
    if (!writeImpl(thiz, kTrueStr, NQ_CSTR_LENGTH(kTrueStr)))
      return false;
  }
  else {
    if (!writeImpl(thiz, kFalseStr, NQ_CSTR_LENGTH(kFalseStr)))
      return false;
  }

  thiz->count++;
  return true;
}

bool NQJSONWriter_writeObjectBegin(NQJSONWriter* thiz)
{
  if (!thiz->hasError) {
    if (writeObjectBegin(thiz))
      return true;
    thiz->hasError = true;
  }
  return false;
}

bool NQJSONWriter_writeObjectEnd(NQJSONWriter* thiz)
{
  if (!thiz->hasError) {
    if (writeObjectEnd(thiz))
      return true;
    thiz->hasError = true;
  }
  return false;
}

bool NQJSONWriter_writeArrayBegin(NQJSONWriter* thiz)
{
  if (!thiz->hasError) {
    if (writeArrayBegin(thiz))
      return true;
    thiz->hasError = true;
  }
  return false;
}

bool NQJSONWriter_writeArrayEnd(NQJSONWriter* thiz)
{
  if (!thiz->hasError) {
    if (writeArrayEnd(thiz))
      return true;
    thiz->hasError = true;
  }
  return false;
}

bool NQJSONWriter_writeNull(NQJSONWriter* thiz)
{
  if (!thiz->hasError) {
    if (writeNull(thiz))
      return true;
    thiz->hasError = true;
  }
  return false;
}

bool NQJSONWriter_writeChar(NQJSONWriter* thiz, char val)
{
  if (!thiz->hasError) {
    if (writeString(thiz, &val, sizeof(val)))
      return true;
    thiz->hasError = true;
  }
  return false;
}

bool NQJSONWriter_writeCStr(NQJSONWriter* thiz, const char* val)
{
  if (!thiz->hasError) {
    if (writeString(thiz, val, kNoLength))
      return true;
    thiz->hasError = true;
  }
  return false;
}

bool NQJSONWriter_writeCStrWithLength(NQJSONWriter* thiz, const char* val, size_t len)
{
  if (len == kNoLength)
    return false;

  if (!thiz->hasError) {
    if (writeString(thiz, val, len))
      return true;
    thiz->hasError = true;
  }
  return false;
}

bool NQJSONWriter_writeInt64(NQJSONWriter* thiz, int64_t val)
{
  if (!thiz->hasError) {
    if (writeInt64(thiz, val))
      return true;
    thiz->hasError = true;
  }
  return false;
}

bool NQJSONWriter_writeInt32(NQJSONWriter* thiz, int32_t val)
{
  if (!thiz->hasError) {
    if (writeInt64(thiz, val))
      return true;
    thiz->hasError = true;
  }
  return false;
}

bool NQJSONWriter_writeInt16(NQJSONWriter* thiz, int16_t val)
{
  if (!thiz->hasError) {
    if (writeInt64(thiz, val))
      return true;
    thiz->hasError = true;
  }
  return false;
}

bool NQJSONWriter_writeInt8(NQJSONWriter* thiz, int8_t val)
{
  if (!thiz->hasError) {
    if (writeInt64(thiz, val))
      return true;
    thiz->hasError = true;
  }
  return false;
}

bool NQJSONWriter_writeUint64(NQJSONWriter* thiz, uint64_t val)
{
  if (!thiz->hasError) {
    if (writeUint64(thiz, val))
      return true;
    thiz->hasError = true;
  }
  return false;
}

bool NQJSONWriter_writeUint32(NQJSONWriter* thiz, uint32_t val)
{
  if (!thiz->hasError) {
    if (writeUint64(thiz, val))
      return true;
    thiz->hasError = true;
  }
  return false;
}

bool NQJSONWriter_writeUint16(NQJSONWriter* thiz, uint16_t val)
{
  if (!thiz->hasError) {
    if (writeUint64(thiz, val))
      return true;
    thiz->hasError = true;
  }
  return false;
}

bool NQJSONWriter_writeUint8(NQJSONWriter* thiz, uint8_t val)
{
  if (!thiz->hasError) {
    if (writeUint64(thiz, val))
      return true;
    thiz->hasError = true;
  }
  return false;
}

bool NQJSONWriter_writeFloat(NQJSONWriter* thiz, float val)
{
  if (!thiz->hasError) {
    if (writeDouble(thiz, val))
      return true;
    thiz->hasError = true;
  }
  return false;
}

bool NQJSONWriter_writeDouble(NQJSONWriter* thiz, double val)
{
  if (!thiz->hasError) {
    if (writeDouble(thiz, val))
      return true;
    thiz->hasError = true;
  }
  return false;
}


bool NQJSONWriter_writeBool(NQJSONWriter* thiz, bool val)
{
  if (!thiz->hasError) {
    if (writeBool(thiz, val))
      return true;
    thiz->hasError = true;
  }
  return false;
}

bool NQJSONWriter_writeKey(NQJSONWriter* thiz, const char* key)
{
  if (!thiz->hasError) {
    if (writeKey(thiz, key, kNoLength))
      return true;
    thiz->hasError = true;
  }
  return false;
}

bool NQJSONWriter_writeKeyObjectBegin(NQJSONWriter* thiz, const char* key)
{
  if (!thiz->hasError) {
    if (writeKey(thiz, key, kNoLength) && writeObjectBegin(thiz))
      return true;
    thiz->hasError = true;
  }
  return false;
}

bool NQJSONWriter_writeKeyArrayBegin(NQJSONWriter* thiz, const char* key)
{
  if (!thiz->hasError) {
    if (writeKey(thiz, key, kNoLength) && writeArrayBegin(thiz))
      return true;
    thiz->hasError = true;
  }
  return false;
}

bool NQJSONWriter_writeKeyNull(NQJSONWriter* thiz, const char* key)
{
  if (!thiz->hasError) {
    if (writeKey(thiz, key, kNoLength) && writeNull(thiz))
      return true;
    thiz->hasError = true;
  }
  return false;
}

bool NQJSONWriter_writeKeyChar(NQJSONWriter* thiz, const char* key, char val)
{
  if (!thiz->hasError) {
    if (writeKey(thiz, key, kNoLength) && writeString(thiz, &val, sizeof(val)))
      return true;
    thiz->hasError = true;
  }
  return false;
}

bool NQJSONWriter_writeKeyCStr(NQJSONWriter* thiz, const char* key, const char* val)
{
  if (!thiz->hasError) {
    if (writeKey(thiz, key, kNoLength) && writeString(thiz, val, kNoLength))
      return true;
    thiz->hasError = true;
  }
  return false;
}

bool NQJSONWriter_writeKeyCStrWithLength(NQJSONWriter* thiz, const char* key, const char* val, size_t len)
{
  if (len == kNoLength)
    return false;

  if (!thiz->hasError) {
    if (writeKey(thiz, key, len) && writeString(thiz, val, len))
      return true;
    thiz->hasError = true;
  }
  return false;
}

bool NQJSONWriter_writeKeyInt64(NQJSONWriter* thiz, const char* key, int64_t val)
{
  if (!thiz->hasError) {
    if (writeKey(thiz, key, kNoLength) && writeInt64(thiz, val))
      return true;
    thiz->hasError = true;
  }
  return false;
}

bool NQJSONWriter_writeKeyInt32(NQJSONWriter* thiz, const char* key, int32_t val)
{
  if (!thiz->hasError) {
    if (writeKey(thiz, key, kNoLength) && writeInt64(thiz, val))
      return true;
    thiz->hasError = true;
  }
  return false;
}

bool NQJSONWriter_writeKeyInt16(NQJSONWriter* thiz, const char* key, int16_t val)
{
  if (!thiz->hasError) {
    if (writeKey(thiz, key, kNoLength) && writeInt64(thiz, val))
      return true;
    thiz->hasError = true;
  }
  return false;
}

bool NQJSONWriter_writeKeyInt8(NQJSONWriter* thiz, const char* key, int8_t val)
{
  if (!thiz->hasError) {
    if (writeKey(thiz, key, kNoLength) && writeInt64(thiz, val))
      return true;
    thiz->hasError = true;
  }
  return false;
}

bool NQJSONWriter_writeKeyUint64(NQJSONWriter* thiz, const char* key, uint64_t val)
{
  if (!thiz->hasError) {
    if (writeKey(thiz, key, kNoLength) && writeUint64(thiz, val))
      return true;
    thiz->hasError = true;
  }
  return false;
}

bool NQJSONWriter_writeKeyUint32(NQJSONWriter* thiz, const char* key, uint32_t val)
{
  if (!thiz->hasError) {
    if (writeKey(thiz, key, kNoLength) && writeUint64(thiz, val))
      return true;
    thiz->hasError = true;
  }
  return false;
}

bool NQJSONWriter_writeKeyUint16(NQJSONWriter* thiz, const char* key, uint16_t val)
{
  if (!thiz->hasError) {
    if (writeKey(thiz, key, kNoLength) && writeUint64(thiz, val))
      return true;
    thiz->hasError = true;
  }
  return false;
}

bool NQJSONWriter_writeKeyUint8(NQJSONWriter* thiz, const char* key, uint8_t val)
{
  if (!thiz->hasError) {
    if (writeKey(thiz, key, kNoLength) && writeUint64(thiz, val))
      return true;
    thiz->hasError = true;
  }
  return false;
}

bool NQJSONWriter_writeKeyFloat(NQJSONWriter* thiz, const char* key, float val)
{
  if (!thiz->hasError) {
    if (writeKey(thiz, key, kNoLength) && writeDouble(thiz, val))
      return true;
    thiz->hasError = true;
  }
  return false;
}

bool NQJSONWriter_writeKeyDouble(NQJSONWriter* thiz, const char* key, double val)
{
  if (!thiz->hasError) {
    if (writeKey(thiz, key, kNoLength) && writeDouble(thiz, val))
      return true;
    thiz->hasError = true;
  }
  return false;
}

bool NQJSONWriter_writeKeyBool(NQJSONWriter* thiz, const char* key, bool val)
{
  if (!thiz->hasError) {
    if (writeKey(thiz, key, kNoLength) && writeBool(thiz, val))
      return true;
    thiz->hasError = true;
  }
  return false;
}
