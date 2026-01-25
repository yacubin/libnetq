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

const char* NQJSON_objectGetString(NQJSON* json, const char* key)
{
  NQJSON* item = NQJSON_objectGet(json, key);
  return NQJSON_isString(item) ? NQJSON_asString(item) : "";
}
