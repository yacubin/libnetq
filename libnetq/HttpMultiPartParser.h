/*
 * MIT License
 *
 * Copyright (c) 2022-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_HTTPMULTIPARTPARSER_H
#define _LIBNETQ_HTTPMULTIPARTPARSER_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum NQHTTPMultiPartType {
  kNQHTTPMultiPartBegin,
  kNQHTTPMultiPartBoundary,
  kNQHTTPMultiPartFieldName,
  kNQHTTPMultiPartFieldValue,
  kNQHTTPMultiPartBodyPart,
  kNQHTTPMultiPartFinish,
} NQHTTPMultiPartType;

typedef bool (NQHTTPMultiPartParserCallback) (void* userdata, NQHTTPMultiPartType type, const char* data, size_t size);
typedef struct NQHTTPMultiPartParser NQHTTPMultiPartParser;

NQ_EXPORT NQHTTPMultiPartParser* NQHTTPMultiPartParser_create(const char* boundary, NQHTTPMultiPartParserCallback callback, void* userdata);
NQ_EXPORT void NQHTTPMultiPartParser_destroy(NQHTTPMultiPartParser*);

NQ_EXPORT bool NQHTTPMultiPartParser_append(NQHTTPMultiPartParser*, const char* data, size_t size);
NQ_EXPORT bool NQHTTPMultiPartParser_finish(NQHTTPMultiPartParser*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_HTTPMULTIPARTPARSER_H */
