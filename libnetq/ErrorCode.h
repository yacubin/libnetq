/*
 * MIT License
 *
 * Copyright (c) 2020-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_ERRORCODE_H
#define _LIBNETQ_ERRORCODE_H

#include <libnetq/Basic.h>

#if defined(NQ_OS_UNIX) || defined(NQCONFIG_USE_ERRNO_H)
# include <errno.h>
#elif defined(NQ_OS_KERNEL)
# include <linux/errno.h>
#elif defined(NQ_OS_WINDOWS)
# include <winerror.h>
# include <winsock.h>
#endif

typedef int NQErrorCode;

#define NQ_SUCCESS 0
#define NQ_ERROR (-1)

#if defined(NQ_OS_KERNEL) || defined(NQ_OS_UNIX) || defined(NQCONFIG_USE_ERRNO_H)
# define NQ_ENOENT           ENOENT
# define NQ_ENOMEM           ENOMEM
# define NQ_EBUSY            EBUSY
# define NQ_EINVAL           EINVAL
# define NQ_ENOSYS           ENOSYS
# define NQ_ETIMEDOUT        ETIMEDOUT
# define NQ_EINPROGRESS      EINPROGRESS
# define NQ_EWOULDBLOCK      EWOULDBLOCK
# define NQ_ERANGE           ERANGE
# define NQ_EIO              EIO

#if defined(NQ_OS_KERNEL)
# define NQ_ENOTSUP          ENOTSUPP
# define NQ_ERESTARTSYS      ERESTARTSYS
#endif

#if defined(NQ_OS_UNIX) || defined(NQCONFIG_USE_ERRNO_H)
# define NQ_ENOTSUP          ENOTSUP
# define NQ_ERESTARTSYS      EINTR
#endif

#elif defined(NQ_OS_WINDOWS)
# define NQ_ENOENT           ERROR_FILE_NOT_FOUND
# define NQ_ENOMEM           ERROR_NOT_ENOUGH_MEMORY
# define NQ_EBUSY            ERROR_BUSY
# define NQ_EINVAL           ERROR_INVALID_PARAMETER
# define NQ_ENOSYS           ERROR_CALL_NOT_IMPLEMENTED
# define NQ_ETIMEDOUT        ERROR_TIMEOUT
# define NQ_ENOTSUP          ERROR_NOT_SUPPORTED
# define NQ_EINPROGRESS      WSAEINPROGRESS
# define NQ_EWOULDBLOCK      WSAEWOULDBLOCK
# define NQ_ERESTARTSYS      ERROR_OPERATION_ABORTED
# define NQ_ERANGE           ERROR_INSUFFICIENT_BUFFER
# define NQ_EIO              ERROR_IO_DEVICE

#endif

static inline NQErrorCode NQGetLastError(void)
{
  int ec;

#ifdef NQ_OS_KERNEL
  ec = 0;
#endif

#ifdef NQ_OS_WINDOWS
  ec = WSAGetLastError();
#endif

#ifdef NQ_OS_UNIX
  ec = errno;
#endif

  return ec;
}

static inline void NQSetLastError(NQErrorCode ec)
{
#ifdef NQ_OS_KERNEL
  NQ_UNUSED_PARAM(ec);
#endif

#ifdef NQ_OS_WINDOWS
  WSASetLastError(ec);
#endif

#ifdef NQ_OS_UNIX
  errno = ec;
#endif
}

static inline void NQClearLastError(void)
{
#ifdef NQ_OS_WINDOWS
  WSASetLastError(0);
#endif

#ifdef NQ_OS_UNIX
  errno = 0;
#endif
}

#endif /* _LIBNETQ_ERRORCODE_H */
