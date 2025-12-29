/*
 * MIT License
 *
 * Copyright (c) 2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_WEB_JSONRPCTYPES_H
#define _LIBNETQ_WEB_JSONRPCTYPES_H

#include <libnetq/Basic.h>

#define NQ_JSONRPC_VERSION "2.0"
#define NQ_JSONRPC_NAME_JSONRPC "jsonrpc"
#define NQ_JSONRPC_NAME_METHOD "method"
#define NQ_JSONRPC_NAME_PARAMS "params"
#define NQ_JSONRPC_NAME_RESULT "result"
#define NQ_JSONRPC_NAME_ERROR "error"
#define NQ_JSONRPC_NAME_CODE "code"
#define NQ_JSONRPC_NAME_MESSAGE "message"
#define NQ_JSONRPC_NAME_ID "id"

#define NQ_JSONRPC_ECODE_INVALID_REQUEST (-32600)
#define NQ_JSONRPC_ECODE_METHOD_NOT_FOUND (-32601)
#define NQ_JSONRPC_ECODE_INVALID_PARAMS (-32602)
#define NQ_JSONRPC_ECODE_INTERNAL_ERROR (-32603)
#define NQ_JSONRPC_ECODE_PARSE_ERROR (-32700)

#endif /* _LIBNETQ_WEB_JSONRPCTYPES_H */
