/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_XML_XMLPARSER_H
#define _LIBNETQ_XML_XMLPARSER_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQXMLParser NQXMLParser;

NQ_EXPORT const char* NQXMLParser_package(void);
NQ_EXPORT const char* NQXMLParser_version(void);

enum NQXMLParserEventType {
  kNQXMLParserStartElement,
  kNQXMLParserEndElement,
  kNQXMLParserCharacterData,
  kNQXMLParserProcessingInstruction,
  kNQXMLParserComment,
  kNQXMLParserStartCdataSection,
  kNQXMLParserEndCdataSection,
};

struct NQXMLParserAttr {
  const char* name;
  const char* value;
};

struct NQXMLParserStartElement {
  enum NQXMLParserEventType type;
  const char* name;
  const struct NQXMLParserAttr* atts;
};

struct NQXMLParserEndElement {
  enum NQXMLParserEventType type;
  const char* name;
};

struct NQXMLParserCharacterData {
  enum NQXMLParserEventType type;
  const char* data;
  size_t length;
};

struct NQXMLParserProcessingInstruction {
  enum NQXMLParserEventType type;
  const char* target;
  const char* data;
};

struct NQXMLParserComment {
  enum NQXMLParserEventType type;
  const char* data;
};

typedef union NQXMLParserEvent NQXMLParserEvent;
union NQXMLParserEvent {
  enum NQXMLParserEventType type;
  struct NQXMLParserStartElement startElement;
  struct NQXMLParserEndElement endElement;
  struct NQXMLParserCharacterData characterData;
  struct NQXMLParserProcessingInstruction processingInstruction;
  struct NQXMLParserComment comment;
};

typedef bool (*NQXMLParserCallback) (const NQXMLParserEvent* event, void* userdata);

NQ_EXPORT NQXMLParser* NQXMLParser_create(const char* encoding, NQXMLParserCallback callback, void* userdata);
NQ_EXPORT void NQXMLParser_release(NQXMLParser*);
NQ_EXPORT bool NQXMLParser_append(NQXMLParser*, const char* data, size_t size);
NQ_EXPORT bool NQXMLParser_finish(NQXMLParser*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_XML_XMLPARSER_H */
