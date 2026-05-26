/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_IO_UIO_H
#define _LIBNETQ_IO_UIO_H

typedef struct NQIOVec NQIOVec;
struct NQIOVec {
  void* iov_base;
  size_t iov_len;
};

#endif /* _LIBNETQ_IO_UIO_H */
