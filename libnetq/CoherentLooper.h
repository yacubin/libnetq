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

typedef struct CoherentLooperData {
  void* userdata;
  NQExecuteCallback* execute;
  NQDestroyCallback* destroy;
} CoherentLooperData;

typedef struct CoherentLooper CoherentLooper;

CoherentLooper* CoherentLooper_create();
void CoherentLooper_destroy(CoherentLooper*);
void CoherentLooper_stop(CoherentLooper*);
bool CoherentLooper_call(CoherentLooper*, void* userdata, NQExecuteCallback* execute, NQDestroyCallback* destroy);
bool CoherentLooper_wait(CoherentLooper*, CoherentLooperData* data);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_COHERENTLOOPER_H */
