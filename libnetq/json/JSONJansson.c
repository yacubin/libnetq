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

#ifdef NQ_USE_JANSSON_JSON

#include <libnetq/Log.h>
#include <jansson.h>

const char* NQJSON_package(void)
{
  return "jansson";
}

const char* NQJSON_version(void)
{
  return JANSSON_VERSION;
}

NQJSON* NQJSON_fromFile(const char* filename)
{
  json_error_t error;
  NQJSON* thiz = (NQJSON*)json_load_file(filename, 0, &error);
  if (thiz == NULL) {
    NQ_LOGE("%s", error.text);
  }
  return thiz;
}

NQJSON* NQJSON_parse(const char* text)
{
  json_error_t error;
  NQJSON* thiz = (NQJSON*)json_loads(text, JSON_DECODE_ANY, &error);
  if (thiz == NULL) {
    NQ_LOGE("%s", error.text);
  }
  return thiz;
}

NQJSON* NQJSON_parse2(const char* text, size_t length)
{
  json_error_t error;
  NQJSON* thiz = (NQJSON*)json_loadb(text, length, JSON_DECODE_ANY, &error);
  if (thiz == NULL) {
    NQ_LOGE("%s", error.text);
  }
  return thiz;
}

NQJSON* NQJSON_clone(const NQJSON* json, bool deep)
{
  return (NQJSON*)(deep ? json_deep_copy((json_t*)json) : json_copy((json_t*)json));
}

NQJSON* NQJSON_createNull(void)
{
  return (NQJSON*)json_null();
}

NQJSON* NQJSON_createTrue(void)
{
  return (NQJSON*)json_true();
}

NQJSON* NQJSON_createFalse(void)
{
  return (NQJSON*)json_false();
}

NQJSON* NQJSON_createBool(bool value)
{
  return (NQJSON*)json_boolean(value);
}

NQJSON* NQJSON_createDouble(double value)
{
  return (NQJSON*)json_real(value);
}

NQJSON* NQJSON_createInt64(int64_t value)
{
  return (NQJSON*)json_integer(value);
}

NQJSON* NQJSON_createStringRef(const char* value)
{
  return (NQJSON*)json_string(value);
}

NQJSON* NQJSON_createArrayRef(void)
{
  return (NQJSON*)json_array();
}

NQJSON* NQJSON_createObjectRef(void)
{
  return (NQJSON*)json_object();
}

bool NQJSON_isNull(const NQJSON* json)
{
  return json_is_null((json_t*)json);
}

bool NQJSON_isFalse(const NQJSON* json)
{
  return json_is_false((json_t*)json);
}

bool NQJSON_isTrue(const NQJSON* json)
{
  return json_is_true((json_t*)json);
}

bool NQJSON_isBool(const NQJSON* json)
{
  return json_is_boolean((json_t*)json);
}

bool NQJSON_isNumber(const NQJSON* json)
{
  return json_is_number((json_t*)json);
}

bool NQJSON_isDouble(const NQJSON* json)
{
  return json_is_real((json_t*)json);
}

bool NQJSON_isInt64(const NQJSON* json)
{
  return json_is_integer((json_t*)json);
}

bool NQJSON_isString(const NQJSON* json)
{
  return json_is_string((json_t*)json);
}

bool NQJSON_isArray(const NQJSON* json)
{
  return json_is_array((json_t*)json);
}

bool NQJSON_isObject(const NQJSON* json)
{
  return json_is_object((json_t*)json);
}

bool NQJSON_asBool(const NQJSON* json)
{
  return json_boolean_value((json_t*)json);
}

double NQJSON_asDouble(const NQJSON* json)
{
  return json_is_real((json_t*)json) ? json_real_value((json_t*)json) : (double)json_integer_value((json_t*)json);
}

int64_t NQJSON_asInt64(const NQJSON* json)
{
  return json_is_integer((json_t*)json) ? json_integer_value((json_t*)json) : (int64_t)json_real_value((json_t*)json);
}

const char* NQJSON_asString(const NQJSON* json)
{
  return json_string_value((json_t*)json);
}

size_t NQJSON_stringLength(const NQJSON* json)
{
  return json_string_length((json_t*)json);
}

size_t NQJSON_arraySize(const NQJSON* json)
{
  return json_array_size((json_t*)json);
}

NQJSON* NQJSON_arrayAt(NQJSON* json, size_t index)
{
  return (NQJSON*)json_array_get((json_t*)json, index);
}

bool NQJSON_arrayAdd(NQJSON* json, NQJSON* item)
{
  return json_array_append((json_t*)json, (json_t*)item) == 0;
}

NQJSON* NQJSON_objectGet(NQJSON* json, const char* key)
{
  return (NQJSON*)json_object_get((json_t*)json, key);
}

bool NQJSON_objectSet(NQJSON* json, const char* key, NQJSON* item)
{
  return json_object_set((json_t*)json, key, (json_t*)item) == 0;
}

bool NQJSON_objectSetDouble(NQJSON* json, const char* key, double value)
{
  return json_object_set_new((json_t*)json, key, json_real(value)) == 0;
}

bool NQJSON_objectSetString(NQJSON* json, const char* key, const char* value)
{
  return json_object_set_new((json_t*)json, key, json_string(value)) == 0;
}

NQJSON_ObjectIter* NQJSON_objectIterFirst(NQJSON* json)
{
  return json_object_iter((json_t*)json);
}

NQJSON_ObjectIter* NQJSON_objectIterNext(NQJSON* json, NQJSON_ObjectIter* iter)
{
  return json_object_iter_next((json_t*)json, iter);
}

const char* NQJSON_objectIterKey(NQJSON_ObjectIter* iter)
{
  return json_object_iter_key(iter);
}

NQJSON* NQJSON_objectIterValue(NQJSON_ObjectIter* iter)
{
  return (NQJSON*)json_object_iter_value(iter);
}

void NQJSON_release(NQJSON* json)
{
  json_decref((json_t*)json);
}

bool NQJSON_isEqual(NQJSON* a, NQJSON* b)
{
  return json_equal((json_t*)a, (json_t*)b) == 1;
}
#endif
