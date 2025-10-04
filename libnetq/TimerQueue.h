/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_TIMERQUEUE_H
#define _LIBNETQ_TIMERQUEUE_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NQ_TIMER_INVALID 0

typedef uint32_t NQTimerIdentifier;
typedef struct NQTimerData NQTimerData;
typedef struct NQTimerQueue NQTimerQueue;

struct NQTimerData {
  void* userdata;
  void (*handle) (void* userdata);
  void (*destroy) (void* userdata);
};

enum {
  NQ_TIMER_WAIT,
  NQ_TIMER_REPEATE,
  NQ_TIMER_REMOVE,
};

NQ_EXPORT NQTimerQueue* NQTimerQueue_create(void);
NQ_EXPORT void NQTimerQueue_destroy(NQTimerQueue*);

NQ_EXPORT int64_t NQTimerQueue_timeout(NQTimerQueue*, int64_t now);
// TODO: return timeout
NQ_EXPORT int NQTimerQueue_nextFired(NQTimerQueue*, int64_t now, NQTimerData* data);

NQ_EXPORT NQTimerIdentifier NQTimerQueue_startTimer(NQTimerQueue*, bool isInterval, int64_t now, int64_t timeout, NQTimerData* data);
NQ_EXPORT bool NQTimerQueue_stopTimer(NQTimerQueue*, NQTimerIdentifier identifier, NQTimerData* data);

NQ_EXPORT bool NQTimerQueue_isValid(NQTimerQueue*, NQTimerIdentifier identifier);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_TIMERQUEUE_H */
