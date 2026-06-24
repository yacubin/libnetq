/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_NET_KERNEL_SOCKETHANDLE_H
#define _LIBNETQ_NET_KERNEL_SOCKETHANDLE_H

#include <libnetq/Basic.h>

#ifdef NQ_OS_KERNEL

#include <linux/net.h>

typedef struct socket* NQSocketHandle;
#define NQ_INVALID_SOCKET NULL

#endif

#endif /* _LIBNETQ_NET_KERNEL_SOCKETHANDLE_H */
