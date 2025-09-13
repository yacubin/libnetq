/*
 * MIT License
 *
 * Copyright (c) 2023-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/HttpMethod.h"

#include <libnetq/CStrBase.h>

NQHttpMethod NQHttpMethodParse(const char* str)
{
  if (!strcmp(str, NQ_HTTP_GET))
    return kNQHttpGetMethod;
  else if (!strcmp(str, NQ_HTTP_HEAD))
    return kNQHttpHeadMethod;
  else if (!strcmp(str, NQ_HTTP_POST))
    return kNQHttpPostMethod;
  else if (!strcmp(str, NQ_HTTP_PUT))
    return kNQHttpPutMethod;
  else if (!strcmp(str, NQ_HTTP_DELETE))
    return kNQHttpDeleteMethod;
  else if (!strcmp(str, NQ_HTTP_CONNECT))
    return kNQHttpConnectMethod;
  else if (!strcmp(str, NQ_HTTP_OPTIONS))
    return kNQHttpOptionsMethod;
  else if (!strcmp(str, NQ_HTTP_TRACE))
    return kNQHttpTraceMethod;
  else if (!strcmp(str, NQ_HTTP_PATCH))
    return kNQHttpPatchMethod;

  return kNQHttpUnknownMethod;
}

const char* NQHttpMethodToCStr(NQHttpMethod method)
{
  switch (method) {
  case kNQHttpGetMethod:
    return NQ_HTTP_GET;
  case kNQHttpHeadMethod:
    return NQ_HTTP_HEAD;
  case kNQHttpPostMethod:
    return NQ_HTTP_POST;
  case kNQHttpPutMethod:
    return NQ_HTTP_PUT;
  case kNQHttpDeleteMethod:
    return NQ_HTTP_DELETE;
  case kNQHttpConnectMethod:
    return NQ_HTTP_CONNECT;
  case kNQHttpOptionsMethod:
    return NQ_HTTP_OPTIONS;
  case kNQHttpTraceMethod:
    return NQ_HTTP_TRACE;
  case kNQHttpPatchMethod:
    return NQ_HTTP_PATCH;
  default:
    return NULL;
  }
}
