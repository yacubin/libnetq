/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_THREADDATA_H
#define _LIBNETQ_THREADDATA_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif
  
typedef struct NQThreadData NQThreadData;
typedef void (*NQThreadDataCallback) (void*);

NQ_EXPORT NQThreadData* NQThreadData_create(NQThreadDataCallback callback);
NQ_EXPORT void NQThreadData_destroy(NQThreadData* data);

NQ_EXPORT void* NQThreadData_get(NQThreadData* data);
NQ_EXPORT bool NQThreadData_set(NQThreadData* data, const void* ptr);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_THREADDATA_H */
