/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_JSON_JSON_H
#define _LIBNETQ_JSON_JSON_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQJSON NQJSON;

NQ_EXPORT const char* NQJSON_package(void);
NQ_EXPORT const char* NQJSON_version(void);

NQ_EXPORT NQJSON* NQJSON_parse(const char* text);
NQ_EXPORT NQJSON* NQJSON_parse2(const char* text, size_t length);
NQ_EXPORT NQJSON* NQJSON_clone(NQJSON*, bool deep);

NQ_EXPORT NQJSON* NQJSON_createNull(void);
NQ_EXPORT NQJSON* NQJSON_createTrue(void);
NQ_EXPORT NQJSON* NQJSON_createFalse(void);
NQ_EXPORT NQJSON* NQJSON_createBool(bool value);
NQ_EXPORT NQJSON* NQJSON_createDouble(double value);
NQ_EXPORT NQJSON* NQJSON_createInt64(int64_t value);
NQ_EXPORT NQJSON* NQJSON_createStringRef(const char* value);
NQ_EXPORT NQJSON* NQJSON_createArrayRef(void);
NQ_EXPORT NQJSON* NQJSON_createObjectRef(void);

NQ_EXPORT bool NQJSON_isNull(const NQJSON*);
NQ_EXPORT bool NQJSON_isFalse(const NQJSON*);
NQ_EXPORT bool NQJSON_isTrue(const NQJSON*);
NQ_EXPORT bool NQJSON_isBool(const NQJSON*);
NQ_EXPORT bool NQJSON_isNumber(const NQJSON*);
NQ_EXPORT bool NQJSON_isDouble(const NQJSON*);
NQ_EXPORT bool NQJSON_isInt64(const NQJSON*);
NQ_EXPORT bool NQJSON_isString(const NQJSON*);
NQ_EXPORT bool NQJSON_isArray(const NQJSON*);
NQ_EXPORT bool NQJSON_isObject(const NQJSON*);

NQ_EXPORT bool NQJSON_asBool(const NQJSON*);
NQ_EXPORT double NQJSON_asDouble(const NQJSON*);
NQ_EXPORT int64_t NQJSON_asInt64(const NQJSON*);
NQ_EXPORT const char* NQJSON_asString(const NQJSON* json);

NQ_EXPORT size_t NQJSON_stringLength(const NQJSON*);

NQ_EXPORT size_t NQJSON_arraySize(const NQJSON*);
NQ_EXPORT NQJSON* NQJSON_arrayAt(NQJSON*, size_t index);
NQ_EXPORT bool NQJSON_arrayAdd(NQJSON*, NQJSON* item);
NQ_EXPORT NQJSON* NQJSON_objectGet(NQJSON*, const char* key);
NQ_EXPORT const char* NQJSON_objectGetString(NQJSON*, const char* key);
NQ_EXPORT bool NQJSON_objectSet(NQJSON* json, const char* key, NQJSON* item);
NQ_EXPORT bool NQJSON_objectSetDouble(NQJSON* json, const char* key, double value);
NQ_EXPORT bool NQJSON_objectSetString(NQJSON* json, const char* key, const char* value);

typedef void NQJSON_ObjectIter;

NQ_EXPORT NQJSON_ObjectIter* NQJSON_objectIterFirst(NQJSON*);
NQ_EXPORT NQJSON_ObjectIter* NQJSON_objectIterNext(NQJSON*, NQJSON_ObjectIter* iter);
NQ_EXPORT const char* NQJSON_objectIterKey(NQJSON_ObjectIter* iter);
NQ_EXPORT NQJSON* NQJSON_objectIterValue(NQJSON_ObjectIter* iter);

NQ_EXPORT void NQJSON_release(NQJSON*);
NQ_EXPORT bool NQJSON_isEqual(NQJSON* a, NQJSON* b);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_JSON_JSON_H */
