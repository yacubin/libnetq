/*
 * MIT License
 *
 * Copyright (c) 2021-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_DISPATCHQUEUE_H
#define _LIBNETQ_DISPATCHQUEUE_H

#include <libnetq/List.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQDispatchData NQDispatchData;
typedef struct NQDispatchQueue NQDispatchQueue;

struct NQDispatchData {
  void* userdata;
  void (*handle) (void* userdata);
  void (*destroy) (void* userdata);
};

struct NQDispatchQueue {
  NQListHead workList;
  NQListHead freeList;
  uint32_t size;
  uint32_t total;
};

NQ_EXPORT NQDispatchQueue* NQDispatchQueue_create(size_t capacity);
NQ_EXPORT void NQDispatchQueue_init(NQDispatchQueue*);
NQ_EXPORT void NQDispatchQueue_finalize(NQDispatchQueue*);
NQ_EXPORT void NQDispatchQueue_destroy(NQDispatchQueue* queue);

NQ_EXPORT size_t NQDispatchQueue_size(const NQDispatchQueue* queue);
NQ_EXPORT bool NQDispatchQueue_isEmpty(const NQDispatchQueue* queue);
NQ_EXPORT bool NQDispatchQueue_push(NQDispatchQueue* queue, NQDispatchData* data);
NQ_EXPORT bool NQDispatchQueue_shift(NQDispatchQueue* queue, NQDispatchData* data);

NQ_EXPORT bool NQDispatchQueue_performOnce(NQDispatchQueue* queue);
NQ_EXPORT size_t NQDispatchQueue_performAll(NQDispatchQueue* queue);
NQ_EXPORT bool NQDispatchQueue_cleanOnce(NQDispatchQueue* queue);
NQ_EXPORT size_t NQDispatchQueue_cleanAll(NQDispatchQueue* queue);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_DISPATCHQUEUE_H */
