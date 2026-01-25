/*
 * MIT License
 *
 * Copyright (c) 2020-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/SocketHandle.h"

#include <libnetq/Malloc.h>
#include <libnetq/CStrBase.h> // for memset
#include <libnetq/Limits.h>
#include <libnetq/Assert.h>

#ifdef NQ_SYS_LINUX
#include <linux/socket.h>
#include <linux/net.h> // SOCK_STREAM
#include <uapi/linux/tcp.h> // TCP_NODELAY
#include <uapi/linux/in.h> // sockaddr_in
#include <uapi/linux/in6.h> // sockaddr_in6

typedef int socklen_t;

static inline int NQSocketGetLastError(void)
{
  return errno;
}

#define NQSocketInit() ((void)0)
#define NQSocketOpenImpl(domain, type, protocol) \
  ({ (void)(domain); (void)(type); (void)(protocol); -1; })
#define NQSocketConnectImpl(handle, addr, len) \
  ({ (void)(handle); (void)(addr); (void)(len); -1; })
#define NQSocketAcceptImpl(handle, addr, len) \
  ({ (void)(handle); (void)(addr); (void)(len); -1; })
#define NQSocketCloseImpl(handle) ((void)(handle))

#define _SD_RECV    0
#define _SD_SEND    0
#define _SD_BOTH    0

static int shutdown(int, int) { return -1; }
static int getsockopt(int, int, int, void*, socklen_t*) { return -1; }
static int setsockopt(int, int, int, const void*, socklen_t) { return -1; }
static int fcntl(int, int, ...) { return -1; }
static int bind(int, const struct sockaddr*, socklen_t) { return -1; }
static int listen(int, int) { return -1; }

#define FD_SETSIZE 1024

#endif

#ifdef NQ_OS_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <libnetq/Once.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define NQ_SOCKET_ERROR SOCKET_ERROR

#define NQSocketOpenImpl(domain, type, protocol) WSASocketW(domain, type, protocol, NULL, 0, 0)
#define NQSocketConnectImpl(handle, addr, len) WSAConnect(handle, addr, len, NULL, NULL, NULL, NULL)
#define NQSocketAcceptImpl(handle, addr, len) WSAAccept(handle, addr, len, NULL, 0)
#define NQSocketCloseImpl(handle) closesocket(handle)
#define NQSocketIoctlImpl ioctlsocket

#define _SD_RECV    SD_RECEIVE
#define _SD_SEND    SD_SEND
#define _SD_BOTH    SD_BOTH

NQ_STATIC_ASSERT(sizeof(NQSocketHandle) == sizeof(SOCKET), "Socket size did not match in Windows");

static WSADATA s_wsadata;
static NQOnce s_once = NQ_ONCE_INIT;
static void WSAStartupInit(void)
{
  int status = WSAStartup(MAKEWORD(2, 2), &s_wsadata);
  NQ_ASSERT_UNUSED(status, !status);
}

static inline int NQSocketGetLastError(void)
{
  return WSAGetLastError();
}

#define NQSocketInit() (NQOnce_call(&s_once, &WSAStartupInit))

#endif

#ifdef NQ_OS_UNIX
#include <sys/ioctl.h>   // For: ioctl
#include <sys/socket.h>
#include <netinet/in.h>  // For: IPPROTO_TCP
#include <netinet/tcp.h> // For: TCP_NODELAY
#include <fcntl.h>       // For: fcntl
#include <unistd.h>      // For: close
#include <errno.h>

#define NQ_SOCKET_ERROR (-1)

#define NQSocketOpenImpl(domain, type, protocol) socket(domain, type, protocol)
#define NQSocketConnectImpl(handle, addr, len) connect(handle, addr, len)
#define NQSocketAcceptImpl(handle, addr, len) accept(handle, addr, len)
#define NQSocketCloseImpl(handle) close(handle)
#define NQSocketIoctlImpl ioctl

#define _SD_RECV    SHUT_RD
#define _SD_SEND    SHUT_WR
#define _SD_BOTH    SHUT_RDWR

static inline int NQSocketGetLastError(void)
{
  return errno;
}

#define NQSocketInit() ((void)0)

#endif

typedef union NQSockAddr {
  struct sockaddr sa;
  struct sockaddr_in in4;
  struct sockaddr_in6 in6;
  struct sockaddr_storage storage;
} NQSockAddr;

static int toComposeDomain(int domain)
{
  switch (domain) {
  case NQ_AF_INET4:
    return AF_INET;
  case NQ_AF_INET6:
    return AF_INET6;
  default:
    NQ_ASSERT_NOT_REACHED();
  }
  return domain;
}

static int toComposeType(int type)
{
  switch (type) {
  case NQ_SOCK_STREAM:
    return SOCK_STREAM;
  case NQ_SOCK_DGRAM:
    return SOCK_DGRAM;
  default:
    NQ_ASSERT_NOT_REACHED();
  }
  return type;
}

static int toComposeProtocol(int protocol)
{
  switch (protocol) {
  case NQ_IPPROTO_IP:
    return IPPROTO_IP;
  case NQ_IPPROTO_TCP:
    return IPPROTO_TCP;
  case NQ_IPPROTO_UDP:
    return IPPROTO_UDP;
  default:
    NQ_ASSERT_NOT_REACHED();
  }
  return protocol;
}

static void NQIPv4EndPoint_initWithInet4(NQIPv4EndPoint* thiz, const struct sockaddr_in* addr)
{
  memcpy(thiz->address.data, &addr->sin_addr.s_addr, 4);
  thiz->port = htons(addr->sin_port);
}

static socklen_t NQIPv4EndPoint_toInet4(const NQIPv4EndPoint* ep, struct sockaddr_in* result)
{
  memset(result, 0, sizeof(*result));
  result->sin_family = AF_INET;
  memcpy(&result->sin_addr.s_addr, ep->address.data, 4);
  result->sin_port = htons(ep->port);
  return sizeof(*result);
}

static void NQIPv6EndPoint_initWithInet6(NQIPv6EndPoint* thiz, const struct sockaddr_in6* addr)
{
  memcpy(&thiz->address.data[0], &addr->sin6_addr.s6_addr[0], sizeof(addr->sin6_addr.s6_addr));
  thiz->port = htons(addr->sin6_port);
}

static socklen_t NQIPv6EndPoint_toInet6(const NQIPv6EndPoint* ep, struct sockaddr_in6* result)
{
  memset(result, 0, sizeof(*result));
  result->sin6_family = AF_INET6;
  memcpy(&result->sin6_addr.s6_addr[0], &ep->address.data[0], sizeof(ep->address.data));
  result->sin6_port = htons(ep->port);
  return sizeof(*result);
}

static bool NQEndPoint_initWithInet(NQEndPoint* thiz, const struct sockaddr* addr, socklen_t len)
{
  if (addr->sa_family == AF_INET) {
    if (len < sizeof(struct sockaddr_in))
      return false;
    thiz->family = NQ_AF_INET4;
    NQIPv4EndPoint_initWithInet4(&thiz->ip4ep, (const struct sockaddr_in*)addr);
    return true;
  }

  if (addr->sa_family == AF_INET6) {
    if (len < sizeof(struct sockaddr_in6))
      return false;
    thiz->family = NQ_AF_INET6;
    NQIPv6EndPoint_initWithInet6(&thiz->ip6ep, (const struct sockaddr_in6*)addr);
    return true;
  }

  return false;
}

static socklen_t NQEndPoint_toInet(const NQEndPoint* thiz, struct sockaddr* addr, socklen_t len)
{
  if (thiz->family == NQ_AF_INET4) {
    if (len < sizeof(struct sockaddr_in))
      return 0;
    return NQIPv4EndPoint_toInet4(&thiz->ip4ep, (struct sockaddr_in*)addr);
  }

  if (thiz->family == NQ_AF_INET6) {
    if (len < sizeof(struct sockaddr_in6))
      return 0;
    return NQIPv6EndPoint_toInet6(&thiz->ip6ep, (struct sockaddr_in6*)addr);
  }

  return 0;
}

int NQSocketOpen(int domain, int type, int protocol, NQSocketHandle* result)
{
  NQSocketInit();

  domain = toComposeDomain(domain);
  type = toComposeType(type);
  protocol = toComposeProtocol(protocol);

  NQSocketHandle handle = NQSocketOpenImpl(domain, type, protocol);
  if (handle == NQ_INVALID_SOCKET)
    return -NQSocketGetLastError();

  *result = handle;
  return 0;
}

int NQSocketSend(NQSocketHandle handle, const uint8_t* buf, size_t len, int flags)
{
  if (NQ_INT32_MAX < len)
    len = NQ_INT32_MAX;

#ifdef NQ_SYS_LINUX
  return -1;
#endif

#ifdef NQ_OS_WINDOWS
  return send(handle, (const char*)buf, (int)len, flags);
#endif

#ifdef NQ_OS_UNIX
#ifdef NQ_OS_LINUX
  flags |= MSG_NOSIGNAL;
#endif
  ssize_t result = send(handle, buf, len, flags);
  NQ_ASSERT(result <= NQ_INT32_MAX);
  return (int)result;
#endif
}

int NQSocketRecv(NQSocketHandle handle, uint8_t* buf, size_t len, int flags)
{
  if (NQ_INT32_MAX < len)
    len = NQ_INT32_MAX;

#ifdef NQ_SYS_LINUX
  return -1;
#endif

#ifdef NQ_OS_WINDOWS
  return recv(handle, (char*)buf, (int)len, flags);
#endif

#ifdef NQ_OS_UNIX
#ifdef NQ_OS_LINUX
  flags |= MSG_NOSIGNAL;
#endif
  // flags |= MSG_DONTWAIT; // TODO
  ssize_t result = recv(handle, buf, len, flags);
  NQ_ASSERT(result <= NQ_INT32_MAX);
  return (int)result;
#endif
}

void NQSocketClose(NQSocketHandle handle)
{
  NQSocketCloseImpl(handle);
}

int NQSocketShutdown(NQSocketHandle handle, int how)
{
  switch (how) {
  case NQ_SD_RECV:
    how = _SD_RECV;
    break;

  case NQ_SD_SEND:
    how = _SD_SEND;
    break;

  case NQ_SD_BOTH:
    how = _SD_BOTH;
    break;

  default:
    NQ_ASSERT_NOT_REACHED();
    break;
  }

  return shutdown(handle, how);
}

bool NQSocketGetOpt(NQSocketHandle handle, int level, int id, void* value, uint32_t* length)
{
  NQ_STATIC_ASSERT(sizeof(*length) == sizeof(socklen_t), "Size of socklen_t not valid");
  if (!getsockopt(handle, level, id, (char*)value, (socklen_t*)length))
    return true;
  return false;
}

bool NQSocketSetOpt(NQSocketHandle handle, int level, int id, const void* value, uint32_t length)
{
  NQ_STATIC_ASSERT(sizeof(length) == sizeof(socklen_t), "Size of socklen_t not valid");
  if (!setsockopt(handle, level, id, (const char*)value, (socklen_t)length))
    return true;
  return false;
}

bool NQSocketSetNonBlocking(NQSocketHandle handle, bool blocking)
{
#if defined(NQ_OS_WINDOWS)
  unsigned long opt = blocking ? 1 : 0;
  if (NQSocketIoctlImpl(handle, FIONBIO, &opt) != NQ_SOCKET_ERROR)
    return true;

#elif defined(O_NONBLOCK)
  /* FIXME: O_NONBLOCK is defined but broken on SunOS 4.1.x and AIX 3.2.5. */
  int flags = fcntl(handle, F_GETFL, 0);
  if (flags == -1)
    flags = 0;

  if (blocking)
    flags |= O_NONBLOCK;
  else
    flags &= ~O_NONBLOCK;

  if (fcntl(handle, F_SETFL, flags) != -1)
    return true;

#else
  int opt = blocking ? 1 : 0;
  if (NQSocketIoctlImpl(handle, FIONBIO, ((char*)&opt)) != -1)
    return true;

#endif

  return false;
}

static inline bool getBoolValue(const void* value, uint32_t length, bool* success)
{
  const uint8_t* start = (const uint8_t*)value;
  const uint8_t* end = start + length;

  if (success)
    *success = (length != 0);

  while (start < end) {
    if (*start)
      return true;
    start++;
  }

  return false;
}

static int getoptlevel(int opt)
{
  switch (opt) {
  case NQ_SOCKOPT_ERROR:
  case NQ_SOCKOPT_REUSEADDR:
  case NQ_SOCKOPT_BROADCAST:
    return SOL_SOCKET;

  case NQ_SOCKOPT_TCPNODELAY:
    return IPPROTO_TCP;

  default:
    NQ_ASSERT_NOT_REACHED();
    return -1;
  }
}

static int getoptname(int opt)
{
  switch (opt) {
  case NQ_SOCKOPT_ERROR:
    return SO_ERROR;
  case NQ_SOCKOPT_REUSEADDR:
    return SO_REUSEADDR;
  case NQ_SOCKOPT_BROADCAST:
    return SO_BROADCAST;
  case NQ_SOCKOPT_TCPNODELAY:
    return TCP_NODELAY;

  default:
    NQ_ASSERT_NOT_REACHED();
    return -1;
  }
}

bool NQSocketGetDataOpt(NQSocketHandle handle, int opt, void* value, uint32_t* length)
{
  return NQSocketGetOpt(handle, getoptlevel(opt), getoptname(opt), value, length);
}

bool NQSocketSetDataOpt(NQSocketHandle handle, int opt, const void* value, uint32_t length)
{
  if (opt == NQ_SOCKOPT_NONBLOCK) {
    bool success;
    bool val = getBoolValue(value, length, &success);
    return NQSocketSetNonBlocking(handle, val);
  }

  return NQSocketSetOpt(handle, getoptlevel(opt), getoptname(opt), value, length);
}

bool NQSocketGetBoolOpt(NQSocketHandle handle, int opt, bool* value)
{
  if (value == NULL)
    return NQSocketGetDataOpt(handle, opt, NULL, 0);

  int val;
  uint32_t length = sizeof(val);
  bool success = NQSocketGetDataOpt(handle, opt, &val, &length);
  *value = getBoolValue(&val, length, NULL);
  return success;
}

bool NQSocketSetBoolOpt(NQSocketHandle handle, int opt, bool value)
{
  int val = value ? 1 : 0;
  return NQSocketSetDataOpt(handle, opt, &val, sizeof(val));
}

bool NQSocketGetIntOpt(NQSocketHandle handle, int opt, int* value)
{
  uint32_t length = sizeof(*value);
  return NQSocketGetDataOpt(handle, opt, value, &length);
}

bool NQSocketSetIntOpt(NQSocketHandle handle, int opt, int value)
{
  return NQSocketSetDataOpt(handle, opt, &value, sizeof(value));
}

bool NQSocketSetNoDelay(NQSocketHandle handle, bool value)
{
  return NQSocketSetBoolOpt(handle, NQ_SOCKOPT_TCPNODELAY, value);
}

bool NQSocketConnect(NQSocketHandle handle, const NQEndPoint* ep)
{
  if (ep->family == NQ_AF_INET4)
    return NQSocketConnect4(handle, &ep->ip4ep);

  if (ep->family == NQ_AF_INET6)
    return NQSocketConnect6(handle, &ep->ip6ep);

  NQ_ASSERT_NOT_REACHED();
  return false;
}

bool NQSocketConnect4(NQSocketHandle handle, const NQIPv4EndPoint* ep)
{
  NQ_ASSERT(ep);
  struct sockaddr_in addr;
  socklen_t size = NQIPv4EndPoint_toInet4(ep, &addr);
  return NQSocketConnectImpl(handle, (struct sockaddr*)&addr, size) == 0;
}

bool NQSocketConnect6(NQSocketHandle handle, const NQIPv6EndPoint* ep)
{
  NQ_ASSERT(ep);
  struct sockaddr_in6 addr;
  socklen_t size = NQIPv6EndPoint_toInet6(ep, &addr);
  return NQSocketConnectImpl(handle, (struct sockaddr*)&addr, size) == 0;
}

NQSocketHandle NQSocketAccept(NQSocketHandle handle, NQEndPoint* ep)
{
  NQSocketHandle result;
  NQSockAddr addr;
  socklen_t len;

  len = sizeof(addr);
  result = NQSocketAcceptImpl(handle, &addr.sa, &len);
  if (result != NQ_INVALID_SOCKET && ep != NULL) {
    if (!NQEndPoint_initWithInet(ep, &addr.sa, len)) {
      NQ_ASSERT_NOT_REACHED();
      NQSocketCloseImpl(result);
      result = NQ_INVALID_SOCKET;
    }
  }

  return result;
}

int NQSocketBind(NQSocketHandle handle, const NQEndPoint* ep)
{
  NQSockAddr addr;
  socklen_t size = NQEndPoint_toInet(ep, &addr.sa, sizeof(addr));
  return bind(handle, &addr.sa, size);
}

int NQSocketBind4(NQSocketHandle handle, const NQIPv4EndPoint* ep)
{
  NQSockAddr addr;
  socklen_t size = NQIPv4EndPoint_toInet4(ep, &addr.in4);
  return bind(handle, &addr.sa, size);
}

int NQSocketBind6(NQSocketHandle handle, const NQIPv6EndPoint* ep)
{
  NQSockAddr addr;
  socklen_t size = NQIPv6EndPoint_toInet6(ep, &addr.in6);
  return bind(handle, &addr.sa, size);
}

int NQSocketListen(NQSocketHandle handle, int backlog)
{
  return listen(handle, backlog);
}

static inline int NQSocketSendToImpl(NQSocketHandle handle, const uint8_t* buf, size_t len, int flags, const struct sockaddr* addr, socklen_t addrlen)
{
  NQ_ASSERT(NQ_INT32_MAX < len);

#ifdef NQ_SYS_LINUX
  return -1;
#endif

#ifdef NQ_OS_WINDOWS
  return sendto(handle, (const char*)buf, (int)len, flags, addr, addrlen);
#endif

#ifdef NQ_OS_UNIX
#ifdef NQ_OS_LINUX
  flags |= MSG_NOSIGNAL;
#endif
  ssize_t result = sendto(handle, buf, len, flags, addr, addrlen);
  NQ_ASSERT(result <= NQ_INT32_MAX);
  return (int)result;
#endif
}

int NQSocketSendTo(NQSocketHandle handle, const uint8_t* buf, size_t len, int flags, const NQEndPoint* ep)
{
  if (ep->family == NQ_AF_INET4)
    return NQSocketSendTo4(handle, buf, len, flags, &ep->ip4ep);

  if (ep->family == NQ_AF_INET6)
    return NQSocketSendTo6(handle, buf, len, flags, &ep->ip6ep);

  NQ_ASSERT_NOT_REACHED();
  return -1;
}

int NQSocketSendTo4(NQSocketHandle handle, const uint8_t* buf, size_t len, int flags, const NQIPv4EndPoint* ep)
{
  NQ_ASSERT(NQ_INT32_MAX < len);

  struct sockaddr_in addr;
  socklen_t size = NQIPv4EndPoint_toInet4(ep, &addr);

  return NQSocketSendToImpl(handle, buf, len, flags, (struct sockaddr*)&addr, size);
}

int NQSocketSendTo6(NQSocketHandle handle, const uint8_t* buf, size_t len, int flags, const NQIPv6EndPoint* ep)
{
  NQ_ASSERT(NQ_INT32_MAX < len);

  struct sockaddr_in6 addr;
  socklen_t size = NQIPv6EndPoint_toInet6(ep, &addr);

  return NQSocketSendToImpl(handle, buf, len, flags, (struct sockaddr*)&addr, size);
}

int NQSocketPair(int domain, int type, int protocol, NQSocketHandle sock[2])
{
#ifdef NQ_SYS_LINUX
  return -1;
#endif

#ifdef NQ_OS_WINDOWS
  struct sockaddr_in address;
  NQSocketHandle listener;
  socklen_t size = sizeof(address);
  int reuse = 1;
  int lastError;

  NQ_ASSERT(domain == NQ_AF_INET4 || domain == NQ_AF_INET6);

  sock[0] = NQ_INVALID_SOCKET;
  sock[1] = NQ_INVALID_SOCKET;

  lastError = NQSocketOpen(domain, type, protocol, &listener);
  if (lastError)
    return SOCKET_ERROR;

  // ignore errors coming out of this setsockopt.  This is because
  // SO_EXCLUSIVEADDRUSE requires admin privileges on WinXP, but we don't
  // want to force socket pairs to be an admin.
  setsockopt(listener, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (char*)&reuse, (socklen_t)sizeof(reuse));

  address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  address.sin_family = AF_INET;
  address.sin_port = 0;

  for (;;) {
    if (bind(listener, (struct sockaddr*)&address, sizeof(address)) != 0)
      break;

    if (getsockname(listener, (struct sockaddr*)&address, &size) != 0)
      break;

    if (listen(listener, 1) != 0)
      break;

    lastError = NQSocketOpen(domain, type, protocol, &sock[0]);
    if (lastError)
      break;

    if (NQSocketConnectImpl(sock[0], (struct sockaddr*)&address, sizeof(address)) != 0)
      break;

    sock[1] = NQSocketAcceptImpl(listener, (struct sockaddr*)&address, &size);
    if (NQ_INVALID_SOCKET == sock[1])
      break;

    NQSocketCloseImpl(listener);
    return 0;
  }

  lastError = WSAGetLastError();

  if (NQSocketIsValid(listener))
    NQSocketCloseImpl(listener);

  if (NQSocketIsValid(sock[0])) {
    NQSocketCloseImpl(sock[0]);
    sock[0] = NQ_INVALID_SOCKET;
  }

  if (NQSocketIsValid(sock[1])) {
    NQSocketCloseImpl(sock[1]);
    sock[1] = NQ_INVALID_SOCKET;
  }

  WSASetLastError(lastError);
  return SOCKET_ERROR;
#endif

#ifdef NQ_OS_UNIX
  return socketpair(AF_LOCAL, toComposeType(type), toComposeProtocol(protocol), sock);
#endif
}

bool NQSocketIsSelectable(NQSocketHandle handle)
{
  return 0 <= handle && handle < FD_SETSIZE;
}
