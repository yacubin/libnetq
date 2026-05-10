/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_UIO_H
#define _LIBNETQ_UIO_H

#include <libnetq/Basic.h>

#if definded(NQ_OS_KERNEL)
# include <libnetq/io/kernel/UIO.h>
#elif defined(NQ_OS_UNIX)
# include <libnetq/io/unix/UIO.h>
#else
# include <libnetq/io/UIO.h>
#endif

#define NQIOVec_data(iov) (iov)->iov_base
static inline size_t NQIOVec_size(const NQIOVec* iov)
{
  return iov->iov_len;
}

#endif /* _LIBNETQ_UIO_H */
