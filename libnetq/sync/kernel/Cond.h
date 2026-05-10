/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_SYNC_CONDKERNEL_H
#define _LIBNETQ_SYNC_CONDKERNEL_H

#include <libnetq/Mutex.h>

#ifdef NQ_OS_KERNEL

#include <linux/wait.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQCond {
  wait_queue_head_t waitQueue;
} NQCond;

NQ_EXPORT void NQCond_init(NQCond* cond);
NQ_EXPORT void NQCond_finalize(NQCond* cond);
NQ_EXPORT void NQCond_wait(NQCond* cond, NQMutex* mutex);
NQ_EXPORT bool NQCond_waitfor(NQCond* cond, NQMutex* mutex, uint32_t msecs);
NQ_EXPORT void NQCond_signal(NQCond* cond);
NQ_EXPORT void NQCond_broadcast(NQCond* cond);

#ifdef __cplusplus
}
#endif

#endif
#endif /* _LIBNETQ_SYNC_CONDKERNEL_H */
