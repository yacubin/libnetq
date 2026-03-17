/*
 * MIT License
 *
 * Copyright (c) 2020-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_NET_NETETBUFFER_H
#define _LIBNETQ_NET_NETETBUFFER_H

#include <libnetq/List.h>
#include <libnetq/Network.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQNetBufferEntry NQNetBufferEntry;

#define NQ_NETBUFFERENTRIES_MAX 16

struct NQNetBufferEntry {
  NQListHead list;
  uint16_t size;
  uint8_t bytes[NQ_ETHERNET_MTU];
};

typedef struct NQNetBufferQueue {
  NQListHead usedList;
  NQListHead freeList;
  NQNetBufferEntry entries[NQ_NETBUFFERENTRIES_MAX];
} NQNetBufferQueue;

NQ_EXPORT void NQNetBufferQueue_init(NQNetBufferQueue*);
NQ_EXPORT NQNetBufferEntry* NQNetBufferQueue_alloc(NQNetBufferQueue*);
NQ_EXPORT const NQNetBufferEntry* NQNetBufferQueue_shift(NQNetBufferQueue*);
NQ_EXPORT bool NQNetBufferQueue_isEmpty(const NQNetBufferQueue*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_NET_NETETBUFFER_H */
