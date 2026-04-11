/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#define NQ_CLASS_NAME "NQXMLParser"
#define NQ_LOG_TAG NQ_CLASS_NAME

#include "config.h"
#include "libnetq/xml/XMLParser.h"

#ifdef NQ_USE_STUB_XMLPARSER

const char* NQXMLParser_package(void)
{
  return NULL;
}

const char* NQXMLParser_version(void)
{
  return NULL;
}

NQXMLParser* NQXMLParser_create(const char* encoding, NQXMLParserCallback callback, void* userdata)
{
  NQ_UNUSED_PARAM(encoding);
  NQ_UNUSED_PARAM(callback);
  NQ_UNUSED_PARAM(userdata);
  return NULL;
}

void NQXMLParser_release(NQXMLParser* thiz)
{
  NQ_UNUSED_PARAM(thiz);
}

bool NQXMLParser_append(NQXMLParser* thiz, const char* data, size_t size)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(data);
  NQ_UNUSED_PARAM(size);
  return false;
}

bool NQXMLParser_finish(NQXMLParser* thiz)
{
  NQ_UNUSED_PARAM(thiz);
  return false;
}

#endif
