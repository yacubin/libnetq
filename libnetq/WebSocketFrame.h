/*
 * MIT License
 *
 * Copyright (c) 2023-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_WEBSOCKETFRAME_H
#define _LIBNETQ_WEBSOCKETFRAME_H

#include <libnetq/Basic.h>
#include <libnetq/VA.h>

#ifdef __cplusplus
extern "C" {
#endif

enum {
  kNQWebSocketOpcodeContinuation = 0,
  kNQWebSocketOpcodeText = 1,
  kNQWebSocketOpcodeBinary = 2,
  kNQWebSocketOpcodeConnectionClose = 8,
  kNQWebSocketOpcodePing = 9,
  kNQWebSocketOpcodePong = 10,
};

typedef struct NQWebSocketFrame NQWebSocketFrame;
struct NQWebSocketFrame {
  uint8_t opcode;
  bool fin;
  bool hasMask;
  uint32_t mask;
  size_t payloadSize;
  uint8_t* payload;
};

NQ_EXPORT bool NQWebSocketFrameParse(uint8_t* data, size_t size, NQWebSocketFrame* frame);

static inline void NQWebSocketFramePayloadUnmask(const uint8_t mask[4], uint8_t* data, size_t size)
{
  for (size_t i = 0; i < size; i++)
    data[i] ^= mask[i & 3];
}

typedef struct NQWebSocketBuffer NQWebSocketBuffer;
struct NQWebSocketBuffer {
  uint8_t* payloadPtr;
  uint64_t payloadLen;
  uint64_t payloadCapacity;
  uint8_t headerSize;
};

NQ_EXPORT void NQWebSocketBuffer_init(NQWebSocketBuffer*);
NQ_EXPORT void NQWebSocketBuffer_finalize(NQWebSocketBuffer*);
NQ_EXPORT bool NQWebSocketBuffer_reserve(NQWebSocketBuffer*, size_t size);
NQ_EXPORT int NQWebSocketBuffer_printf(NQWebSocketBuffer*, const char* format, ...) NQ_ATTRIBUTE_PRINTF(2, 3);
NQ_EXPORT int NQWebSocketBuffer_vprintf(NQWebSocketBuffer*, const char* format, va_list);
NQ_EXPORT int NQWebSocketBuffer_write(NQWebSocketBuffer*, const void* data, size_t size);
NQ_EXPORT void NQWebSocketBuffer_complete(NQWebSocketBuffer*, uint8_t opcode, bool fin, const uint8_t* mask);
NQ_EXPORT void NQWebSocketBuffer_reset(NQWebSocketBuffer*);

static inline const uint8_t* NQWebSocketBuffer_data(const NQWebSocketBuffer* thiz)
{
  if (thiz->headerSize == 0)
    return NULL;
  return thiz->payloadPtr - thiz->headerSize;
}

static inline size_t NQWebSocketBuffer_size(const NQWebSocketBuffer* thiz)
{
  if (thiz->headerSize == 0)
    return 0;
  return thiz->payloadLen + thiz->headerSize;
}

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_WEBSOCKETFRAME_H */
