/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#define NQ_CLASS_NAME "NQJSON"
#define NQ_LOG_TAG NQ_CLASS_NAME

#include "config.h"
#include "libnetq/json/JSON.h"

#include <libnetq/string/StringPrint.h>
#include <libnetq/json/JSONWriter.h>

bool NQJSON_objectGetBool(const NQJSON* json, const char* name, bool* value)
{
  const NQJSON* item = NQJSON_objectGet(json, name);
  if (!NQJSON_isBool(item))
    return false;
  if (value)
    *value = NQJSON_asBool(item);
  return true;
}

bool NQJSON_objectGetInt64(const NQJSON* json, const char* name, int64_t* value)
{
  const NQJSON* item = NQJSON_objectGet(json, name);
  if (!NQJSON_isInt64(item))
    return false;
  if (value)
    *value = NQJSON_asInt64(item);
  return true;
}

#ifdef NQ_HAS_COMPILER_SSE
bool NQJSON_objectGetDouble(const NQJSON* json, const char* name, double* value)
{
  const NQJSON* item = NQJSON_objectGet(json, name);
  if (!NQJSON_isDouble(item))
    return false;
  if (value)
    *value = NQJSON_asDouble(item);
  return true;
}
#endif

bool NQJSON_objectGetString(const NQJSON* json, const char* name, const char** value)
{
  const NQJSON* item = NQJSON_objectGet(json, name);
  if (!NQJSON_isString(item))
    return false;
  if (value)
    *value = NQJSON_asString(item);
  return true;
}

static bool bufferWrite(void* userdata, const char* characters, size_t length)
{
  NQStringPrint* buffer = (NQStringPrint*)userdata;
  return NQStringPrint_writeAll(buffer, characters, length);
}

NQ_EXPORT bool NQJSON_dump(const NQJSON* json, NQStringPrint* buffer)
{
  NQJSONWriter writer;
  NQJSONWriter_init(&writer, bufferWrite, buffer);
  bool result = NQJSONWriter_writeJSON(&writer, (NQJSON*)json);
  NQJSONWriter_finalize(&writer);
  return result;
}
