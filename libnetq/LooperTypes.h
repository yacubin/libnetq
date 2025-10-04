/*
 * MIT License
 *
 * Copyright (c) 2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_LOOPERTYPES_H
#define _LIBNETQ_LOOPERTYPES_H

enum {
  NQ_LOOPER_NATIVE,
  NQ_LOOPER_SELECTFD,
  NQ_LOOPER_POLLFD,
};

enum {
  NQ_LOOPER_POLL_ERROR = -1,
  NQ_LOOPER_POLL_TIMEOUT = 0,
  NQ_LOOPER_POLL_MESSAGE = 1,
  NQ_LOOPER_POLL_SOURCE = 2,
};

#define NQ_MESSAGE_QUIT 1
#define NQ_MESSAGE_WAKEUP 2
#define NQ_MESSAGE_NORMAL 3

typedef struct NQMessage {
  int type;
} NQMessage;

#endif /* _LIBNETQ_LOOPERTYPES_H */
