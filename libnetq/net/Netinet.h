/*
 * MIT License
 *
 * Copyright (c) 2020-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_NET_NETINET_H
#define _LIBNETQ_NET_NETINET_H

#include <libnetq/Basic.h>

#if defined(NQ_OS_KERNEL)
# include <uapi/linux/in.h>
# include <uapi/linux/tcp.h>
#elif defined(NQ_OS_UNIX)
# include <netinet/in.h>
# include <netinet/tcp.h>
#endif

#if defined(NQ_OS_KERNEL) || defined(NQ_OS_UNIX)

enum {
  NQ_IPPROTO_IP = IPPROTO_IP,
  NQ_IPPROTO_TCP = IPPROTO_TCP,
  NQ_IPPROTO_UDP = IPPROTO_UDP,
};

#define NQ_TCP_NODELAY  TCP_NODELAY

#else

enum {
  NQ_IPPROTO_IP = 0,
  NQ_IPPROTO_TCP = 6,
  NQ_IPPROTO_UDP = 17,
};

#define NQ_TCP_NODELAY  1

#endif

#endif /* _LIBNETQ_NET_NETINET_H */
