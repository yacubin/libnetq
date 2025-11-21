/*
 * MIT License
 *
 * Copyright (c) 2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_PLATFORMPOLL_H
#define _LIBNETQ_PLATFORMPOLL_H

#include <libnetq/Basic.h>

#if defined(NQ_OS_WINDOWS)
#include <winsock2.h>
#endif

#ifdef NQ_OS_UNIX
#include <poll.h>
#endif

#define NQ_POLLIN      POLLIN
#define NQ_POLLPRI     POLLPRI
#define NQ_POLLOUT     POLLOUT

#define NQ_POLLRDNORM  POLLRDNORM
#define NQ_POLLRDBAND  POLLRDBAND
#define NQ_POLLWRNORM  POLLWRNORM
#define NQ_POLLWRBAND  POLLWRBAND

#define NQ_POLLERR     POLLERR
#define NQ_POLLHUP     POLLHUP
#define NQ_POLLNVAL    POLLNVAL

#ifdef NQ_OS_WINDOWS
#define NQPlatformPoll WSAPoll
typedef WSAPOLLFD NQPlatformPollfd;
#endif

#ifdef NQ_OS_UNIX
#define NQPlatformPoll poll
typedef struct pollfd NQPlatformPollfd;
#endif

#endif /* _LIBNETQ_PLATFORMPOLL_H */
