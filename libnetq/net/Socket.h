/*
 * MIT License
 *
 * Copyright (c) 2020-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_NET_SOCKET_H
#define _LIBNETQ_NET_SOCKET_H

#include <libnetq/Basic.h>

#if defined(NQ_OS_KERNEL)
# include <linux/net.h>
#elif defined(NQ_OS_UNIX)
# include <sys/socket.h>
#endif

#if defined(NQ_OS_KERNEL) || defined(NQ_OS_UNIX)

#define NQ_AF_UNSPEC   AF_UNSPEC
#define NQ_AF_INET     AF_INET
#define NQ_AF_INET6    AF_INET6

enum {
  NQ_SOCK_STREAM = SOCK_STREAM,
  NQ_SOCK_DGRAM = SOCK_DGRAM,
};

enum NQSockShutdown {
  NQ_SHUT_RD = SHUT_RD,
  NQ_SHUT_WR = SHUT_WR,
  NQ_SHUT_RDWR = SHUT_RDWR,
};

#define NQ_SO_REUSEADDR  SO_REUSEADDR
#define NQ_SO_ERROR      SO_ERROR
#define NQ_SO_BROADCAST  SO_BROADCAST

#define NQ_SOL_SOCKET    SOL_SOCKET

#else

#define NQ_AF_UNSPEC   0
#define NQ_AF_INET     2
#define NQ_AF_INET6    10

enum {
  NQ_SOCK_STREAM = 1,
  NQ_SOCK_DGRAM = 2,
};

enum {
  NQ_SHUT_RD = 0,
  NQ_SHUT_WR = 1,
  NQ_SHUT_RDWR = 2,
};

#define NQ_SO_REUSEADDR  2
#define NQ_SO_ERROR      4
#define NQ_SO_BROADCAST  6

#define NQ_SOL_SOCKET    1

#endif

#endif /* _LIBNETQ_NET_SOCKET_H */
