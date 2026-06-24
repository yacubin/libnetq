/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_WEB_JSONRPCRESPONSE_H
#define _LIBNETQ_WEB_JSONRPCRESPONSE_H

#include <libnetq/web/WebResponse.h>

#ifdef __cplusplus
extern "C" {
#endif

NQ_EXPORT bool NQWebResponse_writeJsonRpcErrorParams(NQWebResponse* response, int code, const char* message); // TODO: fmt support
NQ_EXPORT bool NQWebResponse_writeJsonRpcError(NQWebResponse* response, uint32_t id, int code, const char* message); // TODO: fmt support

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_WEB_JSONRPCRESPONSE_H */
