/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_NET_WIN32_SOCKETHANDLE_H
#define _LIBNETQ_NET_WIN32_SOCKETHANDLE_H

#include <libnetq/Network.h>

#ifdef NQ_OS_WINDOWS

typedef uintptr_t NQSocketHandle;
#define NQ_INVALID_SOCKET ((uintptr_t)~0)

#endif

#endif /* _LIBNETQ_NET_WIN32_SOCKETHANDLE_H */
