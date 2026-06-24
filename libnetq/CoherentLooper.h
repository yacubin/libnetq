/*
 * MIT License
 *
 * Copyright (c) 2023-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_COHERENTLOOPER_H
#define _LIBNETQ_COHERENTLOOPER_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (NQExecuteCallback) (void* userdata);
typedef void (NQDestroyCallback) (void* userdata);

typedef struct NQCoherentLooperData {
  void* userdata;
  NQExecuteCallback* execute;
  NQDestroyCallback* destroy;
} NQCoherentLooperData;

typedef struct NQCoherentLooper NQCoherentLooper;

NQ_EXPORT NQCoherentLooper* NQCoherentLooper_create(void);
NQ_EXPORT void NQCoherentLooper_destroy(NQCoherentLooper*);
NQ_EXPORT void NQCoherentLooper_stop(NQCoherentLooper*);
NQ_EXPORT bool NQCoherentLooper_call(NQCoherentLooper*, void* userdata, NQExecuteCallback* execute, NQDestroyCallback* destroy);
NQ_EXPORT bool NQCoherentLooper_wait(NQCoherentLooper*, NQCoherentLooperData* data);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_COHERENTLOOPER_H */
