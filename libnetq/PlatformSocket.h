/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_PLATFORMSOCKET_H
#define _LIBNETQ_PLATFORMSOCKET_H

#include <libnetq/Basic.h>

#ifdef NQ_OS_KERNEL
#include <linux/net.h>
typedef struct socket* NQPlatformSocket;
#define NQ_SHUT_RD   SHUT_RD
#define NQ_SHUT_WR   SHUT_WR
#define NQ_SHUT_RDWR SHUT_RDWR
#endif

#ifdef NQ_OS_UNIX
#include <sys/socket.h>
typedef int NQPlatformSocket;
#define NQ_SHUT_RD   SHUT_RD
#define NQ_SHUT_WR   SHUT_WR
#define NQ_SHUT_RDWR SHUT_RDWR
#endif

#ifdef NQ_OS_WINDOWS
#include <winsock2.h>
typedef SOCKET NQPlatformSocket;
#define NQ_SHUT_RD   SD_RECEIVE
#define NQ_SHUT_WR   SD_SEND
#define NQ_SHUT_RDWR SD_BOTH
#endif

#define NQ_SO_ERROR SO_ERROR

#define NQ_SOL_SOCKET SOL_SOCKET

#endif /* _LIBNETQ_PLATFORMSOCKET_H */
