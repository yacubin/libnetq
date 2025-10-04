/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_SOCKETHANDLE_H
#define _LIBNETQ_SOCKETHANDLE_H

#include <libnetq/Network.h>

#ifdef __cplusplus
extern "C" {
#endif
  
#ifdef NQ_SYS_LINUX
typedef int NQSocketHandle;
#define NQ_INVALID_SOCKET (-1)
#endif
  
#ifdef NQ_OS_WINDOWS
typedef uintptr_t NQSocketHandle;
#define NQ_INVALID_SOCKET ((uintptr_t)~0)
#endif

#ifdef NQ_OS_UNIX
typedef int NQSocketHandle;
#define NQ_INVALID_SOCKET (-1)
#endif

enum {
  NQ_SOCK_STREAM,
  NQ_SOCK_DGRAM,
};

enum {
  NQ_IPPROTO_IP,
  NQ_IPPROTO_TCP,
  NQ_IPPROTO_UDP,
};

enum {
  NQ_SD_RECV,
  NQ_SD_SEND,
  NQ_SD_BOTH
};

enum {
  NQ_SOCKOPT_NONBLOCK,
  NQ_SOCKOPT_ERROR,
  NQ_SOCKOPT_REUSEADDR,
  NQ_SOCKOPT_BROADCAST,
  NQ_SOCKOPT_TCPNODELAY,
};

#define NQSocketIsValid(handle) ((handle) != NQ_INVALID_SOCKET)

NQ_EXPORT NQSocketHandle NQSocketOpen(int domain, int type, int protocol);
NQ_EXPORT int NQSocketSend(NQSocketHandle handle, const uint8_t* buf, size_t len, int flags);
NQ_EXPORT int NQSocketRecv(NQSocketHandle handle, uint8_t* buf, size_t len, int flags);
NQ_EXPORT void NQSocketClose(NQSocketHandle handle);

NQ_EXPORT int NQSocketShutdown(NQSocketHandle handle, int how);
NQ_EXPORT bool NQSocketGetOpt(NQSocketHandle handle, int level, int id, void* value, uint32_t* length);
NQ_EXPORT bool NQSocketSetOpt(NQSocketHandle handle, int level, int id, const void* value, uint32_t length);
NQ_EXPORT bool NQSocketGetDataOpt(NQSocketHandle handle, int opt, void* value, uint32_t* length);
NQ_EXPORT bool NQSocketSetDataOpt(NQSocketHandle handle, int opt, const void* value, uint32_t length);
NQ_EXPORT bool NQSocketGetBoolOpt(NQSocketHandle handle, int opt, bool* value);
NQ_EXPORT bool NQSocketSetBoolOpt(NQSocketHandle handle, int opt, bool value);
NQ_EXPORT bool NQSocketGetIntOpt(NQSocketHandle handle, int opt, int* value);
NQ_EXPORT bool NQSocketSetIntOpt(NQSocketHandle handle, int opt, int value);

NQ_EXPORT bool NQSocketSetNonBlocking(NQSocketHandle handle, bool value);
NQ_EXPORT bool NQSocketSetNoDelay(NQSocketHandle handle, bool value);

NQ_EXPORT bool NQSocketConnect(NQSocketHandle handle, const NQEndPoint* ep);
NQ_EXPORT bool NQSocketConnect4(NQSocketHandle handle, const NQIPv4EndPoint* ep);
NQ_EXPORT bool NQSocketConnect6(NQSocketHandle handle, const NQIPv6EndPoint* ep);
NQ_EXPORT NQSocketHandle NQSocketAccept(NQSocketHandle handle, NQEndPoint* ep);
NQ_EXPORT int NQSocketBind(NQSocketHandle handle, const NQEndPoint* ep);
NQ_EXPORT int NQSocketBind4(NQSocketHandle handle, const NQIPv4EndPoint* ep);
NQ_EXPORT int NQSocketBind6(NQSocketHandle handle, const NQIPv6EndPoint* ep);
NQ_EXPORT int NQSocketListen(NQSocketHandle handle, int backlog);

NQ_EXPORT int NQSocketSendTo(NQSocketHandle handle, const uint8_t* buf, size_t len, int flags, const NQEndPoint* ep);
NQ_EXPORT int NQSocketSendTo4(NQSocketHandle handle, const uint8_t* buf, size_t len, int flags, const NQIPv4EndPoint* ep);
NQ_EXPORT int NQSocketSendTo6(NQSocketHandle handle, const uint8_t* buf, size_t len, int flags, const NQIPv6EndPoint* ep);

NQ_EXPORT int NQSocketPair(int domain, int type, int protocol, NQSocketHandle socks[2]);
NQ_EXPORT bool NQSocketIsSelectable(NQSocketHandle handle);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_SOCKETHANDLE_H */
