/*
 * MIT License
 *
 * Copyright (c) 2021-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_EVENTWAKEUP_H
#define _LIBNETQ_EVENTWAKEUP_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQEventWakeup {
  int fd[2];
} NQEventWakeup;

NQ_EXPORT bool NQEventWakeup_init(NQEventWakeup*);
NQ_EXPORT void NQEventWakeup_finalize(NQEventWakeup*);

NQ_EXPORT bool NQEventWakeup_sendUint64(NQEventWakeup*, const uint64_t* value);
NQ_EXPORT bool NQEventWakeup_recvUint64(NQEventWakeup*, uint64_t* value);

NQ_EXPORT int NQEventWakeup_handle(NQEventWakeup*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_EVENTWAKEUP_H */
