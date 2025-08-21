/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_ERRORCODE_H
#define _LIBNETQ_ERRORCODE_H

#include <libnetq/Basic.h>

#ifdef NQ_OS_WINDOWS
# include <windows.h>
#endif

#ifdef NQ_OS_UNIX
# include <errno.h>
#endif

typedef int NQErrorCode;

#define NQ_SUCCESS 0
#define NQ_ERROR (-1)

#ifdef NQ_OS_WINDOWS
# define NQ_ERROR_INPROGRESS WSAEINPROGRESS
# define NQ_ERROR_WOULDBLOCK WSAEWOULDBLOCK
#endif

#if defined(NQ_OS_UNIX)
# define NQ_ERROR_INPROGRESS EINPROGRESS
# define NQ_ERROR_WOULDBLOCK EWOULDBLOCK
#endif

static inline NQErrorCode NQGetLastError()
{
  int ec;
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
#ifdef NQ_OS_WINDOWS
  WSASetLastError(ec);
#endif

#ifdef NQ_OS_UNIX
  errno = ec;
#endif
}

static inline void NQClearLastError()
{
#ifdef NQ_OS_WINDOWS
  WSASetLastError(0);
#endif

#ifdef NQ_OS_UNIX
  errno = 0;
#endif
}

#endif /* _LIBNETQ_ERRORCODE_H */
