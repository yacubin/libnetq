/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_NET_POSIX_SOCKETHANDLE_H
#define _LIBNETQ_NET_POSIX_SOCKETHANDLE_H

#include <libnetq/Basic.h>

#ifdef NQ_OS_UNIX

typedef int NQSocketHandle;
#define NQ_INVALID_SOCKET (-1)

#endif

#endif /* _LIBNETQ_NET_POSIX_SOCKETHANDLE_H */
