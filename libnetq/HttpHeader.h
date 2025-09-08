/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_HTTPHEADER_H
#define _LIBNETQ_HTTPHEADER_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NQ_HTTP_CRLF "\r\n"

typedef enum NQHTTPHeaderType {
  kNQHTTPHeaderAcceptEncoding,
  kNQHTTPHeaderAcceptLanguage,
  kNQHTTPHeaderUserAgent,
  kNQHTTPHeaderServer,
  kNQHTTPHeaderAuthorization,
  kNQHTTPHeaderCacheControl,
  kNQHTTPHeaderContentType,
  kNQHTTPHeaderContentDisposition,
  kNQHTTPHeaderCrossOriginOpenerPolicy,
  kNQHTTPHeaderCrossOriginEmbedderPolicy,
  kNQHTTPHeaderHost,
  kNQHTTPHeaderUpgrade,
  kNQHTTPHeaderConnection,
  kNQHTTPHeaderLocation,
  kNQHTTPHeaderOrigin,
  kNQHTTPHeaderPragma,
  kNQHTTPHeaderSecWebSocketAccept,
  kNQHTTPHeaderSecWebSocketKey,
  kNQHTTPHeaderSecWebSocketVersion,
  kNQHTTPHeaderSecWebSocketProtocol,
  kNQHTTPHeaderSecWebSocketExtensions,
} NQHTTPHeaderType;

#define NQHTTP_HEADER_ACCEPT_ENCODING          "Accept-Encoding"
#define NQHTTP_HEADER_ACCEPT_LANGUAGE          "Accept-Language"
#define NQHTTP_HEADER_USER_AGENT               "User-Agent"
#define NQHTTP_HEADER_SERVER                   "Server"
#define NQHTTP_HEADER_AUTHORIZATION            "Authorization"
#define NQHTTP_HEADER_CACHE_CONTROL            "Cache-Control"
#define NQHTTP_HEADER_CONTENT_TYPE             "Content-Type"
#define NQHTTP_HEADER_CONTENT_DISPOSITION      "Content-Disposition"
#define NQHTTP_HEADER_CROSS_ORIGIN_OPENER_POLICY   "Cross-Origin-Opener-Policy"
#define NQHTTP_HEADER_CROSS_ORIGIN_EMBEDDER_POLICY "Cross-Origin-Embedder-Policy"
#define NQHTTP_HEADER_HOST                     "Host"
#define NQHTTP_HEADER_UPGRADE                  "Upgrade"
#define NQHTTP_HEADER_CONNECTION               "Connection"
#define NQHTTP_HEADER_LOCATION                 "Location"
#define NQHTTP_HEADER_ORIGIN                   "Origin"
#define NQHTTP_HEADER_PRAGMA                   "Pragma"
#define NQHTTP_HEADER_SEC_WEBSOCKET_ACCEPT     "Sec-WebSocket-Accept"
#define NQHTTP_HEADER_SEC_WEBSOCKET_KEY        "Sec-WebSocket-Key"
#define NQHTTP_HEADER_SEC_WEBSOCKET_VERSION    "Sec-WebSocket-Version"
#define NQHTTP_HEADER_SEC_WEBSOCKET_PROTOCOL   "Sec-WebSocket-Protocol"
#define NQHTTP_HEADER_SEC_WEBSOCKET_EXTENSIONS "Sec-WebSocket-Extensions"

NQ_EXPORT const char* NQHTTPHeaderTypeToString(NQHTTPHeaderType);

#define NQHTTPHeaderIsNonStandard(s) ((s)[0] == 'X' && (s)[1] == '-')

typedef struct NQHTTPHeader NQHTTPHeader;
struct NQHTTPHeader {
  const char* keyData;
  size_t keySize;
  const char* valData;
  size_t valSize;
};

NQ_EXPORT bool NQHTTPHeaderParse(const char* data, size_t size, NQHTTPHeader* result);

typedef struct NQHttpFormData NQHttpFormData;
struct NQHttpFormData {
  const char* nameData;
  size_t nameSize;
  const char* filenameData;
  size_t filenameSize;
};

NQ_EXPORT bool NQHttpFormDataParse(const char* data, size_t size, NQHttpFormData* result);

NQ_EXPORT const char* NQHttpGetContentBoundary(const char* contentType);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_HTTPHEADER_H */
