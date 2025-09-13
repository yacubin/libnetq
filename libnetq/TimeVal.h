/*
 * MIT License
 *
 * Copyright (c) 2021-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_TIMEVAL_H
#define _LIBNETQ_TIMEVAL_H

#include <libnetq/Time.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct timeval NQTimeVal;

NQ_EXPORT NQTimeVal* NQTimeToTimeVal(NQTime time, NQTimeVal* tv);
NQ_EXPORT NQTime NQTimeValToTime(const NQTimeVal* tv);
NQ_EXPORT int64_t NQTimeValToMsecs(const NQTimeVal* tv);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_TIMEVAL_H */
