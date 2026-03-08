/*
 * MIT License
 *
 * Copyright (c) 2023-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_NET_CHUNKBUFFER_H
#define _LIBNETQ_NET_CHUNKBUFFER_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NQ_CHUNKBUFFER_SIZE (4096 - sizeof(void*) - sizeof(uint16_t))

typedef struct NQChunkBuffer NQChunkBuffer;

struct NQChunkBufferEntry {
  struct NQChunkBufferEntry* next;
  uint16_t size;
  uint8_t data[NQ_CHUNKBUFFER_SIZE];
};

struct NQChunkBuffer {
  struct NQChunkBufferEntry* firstChunk;
  struct NQChunkBufferEntry* lastChunk;
  uint32_t totalSize;
};

NQ_EXPORT void NQChunkBuffer_init(NQChunkBuffer*);
NQ_EXPORT void NQChunkBuffer_finalize(NQChunkBuffer*);
NQ_EXPORT bool NQChunkBuffer_append(NQChunkBuffer*, const void* data, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_NET_CHUNKBUFFER_H */
