/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_NET_STUB_SOCKETHANDLE_H
#define _LIBNETQ_NET_STUB_SOCKETHANDLE_H

#include <libnetq/Network.h>

#ifdef NQ_OS_UNKNOWN

typedef void* NQSocketHandle;
#define NQ_INVALID_SOCKET NULL

#endif

#endif /* _LIBNETQ_NET_STUB_SOCKETHANDLE_H */
