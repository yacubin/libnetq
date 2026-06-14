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

#ifdef NQCONFIG_USE_EXPAT_XMLPARSER

#include <libnetq/Malloc.h>
#include <libnetq/Limits.h>
#include <libnetq/Math.h>
#include <libnetq/Log.h>

#include <expat.h>

struct NQXMLParser {
  XML_Parser parser;
  NQXMLParserCallback callback;
  void* userdata;
};

const char* NQXMLParser_package(void)
{
  return "expat";
}

const char* NQXMLParser_version(void)
{
  return XML_ExpatVersion();
}

static inline void NQXMLParser_notify(NQXMLParser* thiz, const NQXMLParserEvent* event)
{
  if (!thiz->callback(event, thiz->userdata))
    XML_StopParser(thiz->parser, XML_FALSE);
}

static void onStartElement(void* userdata, const XML_Char* name, const XML_Char** atts)
{
  NQXMLParser* thiz = (NQXMLParser*)userdata;

  NQXMLParserEvent event;
  event.startElement.type = kNQXMLParserStartElement;
  event.startElement.name = name;
  event.startElement.atts = (const struct NQXMLParserAttr*)atts;
  NQXMLParser_notify(thiz, &event);
}

static void onEndElement(void* userdata, const XML_Char* name)
{
  NQXMLParser* thiz = (NQXMLParser*)userdata;

  NQXMLParserEvent event;
  event.endElement.type = kNQXMLParserEndElement;
  event.endElement.name = name;
  NQXMLParser_notify(thiz, &event);
}

static void onCharacterData(void* userdata, const XML_Char* data, int length)
{
  NQXMLParser* thiz = (NQXMLParser*)userdata;

  NQXMLParserEvent event;
  event.characterData.type = kNQXMLParserCharacterData;
  event.characterData.data = data;
  event.characterData.length = (size_t)length;
  NQXMLParser_notify(thiz, &event);
}

static void onProcessingInstruction(void* userdata, const XML_Char* target, const XML_Char* data)
{
  NQXMLParser* thiz = (NQXMLParser*)userdata;

  NQXMLParserEvent event;
  event.processingInstruction.type = kNQXMLParserProcessingInstruction;
  event.processingInstruction.target = target;
  event.processingInstruction.data = data;
  NQXMLParser_notify(thiz, &event);
}

static void onComment(void* userdata, const XML_Char* data)
{
  NQXMLParser* thiz = (NQXMLParser*)userdata;

  NQXMLParserEvent event;
  event.comment.type = kNQXMLParserComment;
  event.comment.data = data;
  NQXMLParser_notify(thiz, &event);
}

static void onStartCdataSection(void* userdata)
{
  NQXMLParser* thiz = (NQXMLParser*)userdata;

  NQXMLParserEvent event;
  event.type = kNQXMLParserStartCdataSection;
  NQXMLParser_notify(thiz, &event);
}

static void onEndCdataSection(void* userdata)
{
  NQXMLParser* thiz = (NQXMLParser*)userdata;

  NQXMLParserEvent event;
  event.type = kNQXMLParserEndCdataSection;
  NQXMLParser_notify(thiz, &event);
}

NQXMLParser* NQXMLParser_create(const char* encoding, NQXMLParserCallback callback, void* userdata)
{
  XML_Parser parser = XML_ParserCreate(encoding);
  if (parser == NULL)
    return NULL;

  NQXMLParser* thiz = (NQXMLParser*)NQMalloc(sizeof(*thiz));
  if (thiz == NULL) {
    XML_ParserFree(parser);
    return NULL;
  }

  XML_SetUserData(parser, thiz);
  XML_SetElementHandler(parser, onStartElement, onEndElement);
  XML_SetCharacterDataHandler(parser, onCharacterData);
  XML_SetProcessingInstructionHandler(parser, onProcessingInstruction);
  XML_SetCommentHandler(parser, onComment);
  XML_SetCdataSectionHandler(parser, onStartCdataSection, onEndCdataSection);

  thiz->parser = parser;
  thiz->callback = callback;
  thiz->userdata = userdata;

  return thiz;
}

void NQXMLParser_release(NQXMLParser* thiz)
{
  XML_ParserFree(thiz->parser);
  NQFree(thiz);
}

bool NQXMLParser_append(NQXMLParser* thiz, const char* data, size_t size)
{
  while (size != 0) {
    int len = NQGetMin(NQ_INT_MAX, size);
    int status = XML_Parse(thiz->parser, data, len, XML_FALSE);
    if (status != XML_STATUS_OK) {
      NQ_LOGE("Error code %i line %u column %u", XML_GetErrorCode(thiz->parser), (unsigned)XML_GetCurrentLineNumber(thiz->parser), (unsigned)XML_GetCurrentColumnNumber(thiz->parser));
      return false;
    }
    size -= len;
  }
  return true;
}

bool NQXMLParser_finish(NQXMLParser* thiz)
{
  int status = XML_Parse(thiz->parser, NULL, 0, XML_TRUE);
  if (status != XML_STATUS_OK) {
    NQ_LOGE("Error code %i", XML_GetErrorCode(thiz->parser));
    return false;
  }
  return true;
}

#endif
