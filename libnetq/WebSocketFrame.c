/*
 * MIT License
 *
 * Copyright (c) 2023-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/WebSocketFrame.h"

#include <libnetq/String.h>
#include <libnetq/BufferBuilder.h>
#include <libnetq/Malloc.h>
#include <libnetq/Limits.h>
#include <libnetq/Math.h>
#include <libnetq/Sprintf.h>
#include <libnetq/Assert.h>
#include <libnetq/WebSocketTypes.h>

#define WS_FIN_MASK (0x80)
#define WS_RSV_MASK (0x70)
#define WS_OPCODE_MASK (0x0f)
#define WS_MASK_MASK (0x80)
#define WS_LEN1_MASK (0x7f)

#define NQ_WEBSOCKET_BUFFER_MAX (NQ_UINT64_MAX - NQ_WEBSOCKET_HEADER_MAX)
#define NQ_WEBSOCKET_LENGTH1_MAX 125

bool NQWebSocketFrameParse(uint8_t* data, size_t size, NQWebSocketFrame* frame)
{
  if (size < 2) {
    return false;
  }

  uint8_t flags1 = *data++;
  size--;

  uint8_t flags2 = *data++;
  size--;

  bool fin = (flags1 & WS_FIN_MASK) ? true : false;
  if (flags1 & WS_RSV_MASK) {
    return false;
  }

  uint8_t opcode = flags1 & WS_OPCODE_MASK;
  uint64_t length = flags2 & WS_LEN1_MASK;
  bool hasMask = (flags2 & WS_MASK_MASK) ? true : false;

  switch (opcode) {
  case kNQWebSocketOpcodeText:
  case kNQWebSocketOpcodeBinary:
    break;

  case kNQWebSocketOpcodeConnectionClose:
    if (length != 2)
      return false;
    break;

  default:
    return false;
  }

  if (length == 126) {
    if (size < 2) {
      return false;
    }

    uint16_t len16;
    NQGetUint16BE(data, &len16);
    data += sizeof(len16);
    size -= sizeof(len16);

    length = len16;
  }
  else if (length == 127) {
    if (size < 8) {
      return false;
    }
    return false;
  }

  uint8_t mkey[4] = {0,0,0,0};
  if (hasMask) {
    if (size < sizeof(mkey)) {
      return false;
    }
    (void)memcpy(mkey, data, sizeof(mkey));
    data += sizeof(mkey);
    size -= sizeof(mkey);
  }

  if (length != size) {
    return false;
  }

  if (hasMask) {
    NQWebSocketFramePayloadUnmask(mkey, data, size);
  }

  frame->fin = fin;
  frame->hasMask = hasMask;
  (void)memcpy(&frame->mask, mkey, sizeof(mkey));
  frame->opcode = opcode;
  frame->payload = data;
  frame->payloadSize = size;

  return true;
}

#define toHeaderPtr(payloadPtr) ((payloadPtr) - NQ_WEBSOCKET_HEADER_MAX)
#define toPayloadPtr(headerPtr) ((headerPtr) + NQ_WEBSOCKET_HEADER_MAX)

void NQWebSocketBuffer_init(NQWebSocketBuffer* thiz)
{
  thiz->payloadPtr = NULL;
  thiz->payloadLen = 0;
  thiz->payloadCapacity = 0;
  thiz->headerSize = 0;
}

void NQWebSocketBuffer_finalize(NQWebSocketBuffer* thiz)
{
  if (thiz->payloadPtr) {
    NQFree(toHeaderPtr(thiz->payloadPtr));
  }
}

bool NQWebSocketBuffer_reserve(NQWebSocketBuffer* thiz, size_t size)
{
  if (thiz->payloadCapacity >= size)
    return true;

  size_t allocSize = NQ_WEBSOCKET_HEADER_MAX + size;
  if (allocSize < size)
    return false;

  uint8_t* headerPtr = NQMalloc(allocSize);
  if (headerPtr == NULL)
    return false;

  uint8_t* newPayloadPtr = toPayloadPtr(headerPtr);
  if (thiz->payloadLen != 0) {
    NQ_ASSERT(thiz->payloadPtr != NULL);
    NQ_ASSERT(thiz->payloadCapacity >= thiz->payloadLen);
    memcpy(newPayloadPtr, thiz->payloadPtr, thiz->payloadLen);
    NQFree(toHeaderPtr(thiz->payloadPtr));
  }

  thiz->payloadPtr = newPayloadPtr;
  thiz->payloadCapacity = size;
  return true;
}

int NQWebSocketBuffer_printf(NQWebSocketBuffer* thiz, const char* format, ...)
{
  va_list list;
  va_start(list, format);
  int ret = NQWebSocketBuffer_vprintf(thiz, format, list);
  va_end(list);
  return ret;
}

static bool payloadCapacityResize(NQWebSocketBuffer* thiz, uint64_t newPayloadCapacity)
{
  NQ_ASSERT(newPayloadCapacity > thiz->payloadCapacity);
  NQ_ASSERT(newPayloadCapacity <= NQ_WEBSOCKET_BUFFER_MAX);

  if ((newPayloadCapacity * 2) > newPayloadCapacity)
    newPayloadCapacity *= 2;

  uint8_t* newHeaderPtr = (uint8_t*)NQMalloc(NQ_WEBSOCKET_HEADER_MAX + newPayloadCapacity);
  if (newHeaderPtr == NULL)
    return false;

  uint8_t* newPayloadPtr = toPayloadPtr(newHeaderPtr);
  if (thiz->payloadLen)
    memcpy(newPayloadPtr, thiz->payloadPtr, thiz->payloadLen);
  if (thiz->payloadPtr)
    NQFree(toHeaderPtr(thiz->payloadPtr));

  thiz->payloadCapacity = newPayloadCapacity;
  thiz->payloadPtr = newPayloadPtr;

  return true;
}

int NQWebSocketBuffer_vprintf(NQWebSocketBuffer* thiz, const char* format, va_list list)
{
  va_list listCopy;
  va_copy(listCopy, list);

  int n = vsnprintf((char*)thiz->payloadPtr + thiz->payloadLen, thiz->payloadCapacity - thiz->payloadLen, format, listCopy);
  if (n <= 0)
    return n;

  va_end(listCopy);

  uint64_t newPayloadLen = thiz->payloadLen + n;
  // For newPayloadLen == payloadCapacity need +1 byte for NIL
  if (newPayloadLen < thiz->payloadCapacity) {
    thiz->payloadLen = newPayloadLen;
    return n;
  }

  if (!payloadCapacityResize(thiz, newPayloadLen + 1))
    return -1;

  int ret = vsnprintf((char*)thiz->payloadPtr + thiz->payloadLen, thiz->payloadCapacity - thiz->payloadLen, format, list);
  NQ_ASSERT(ret == n);
  if (ret <= 0)
    return ret;

  thiz->payloadLen += ret;
  NQ_ASSERT(thiz->payloadLen == newPayloadLen);
  return ret;
}

int NQWebSocketBuffer_write(NQWebSocketBuffer* thiz, const void* data, size_t length)
{
  length = NQGetMin(NQ_INT32_MAX, length);

  uint64_t newPayloadLen = thiz->payloadLen + (int)length;
  if (newPayloadLen < thiz->payloadLen) {
    if (thiz->payloadLen >= NQ_WEBSOCKET_BUFFER_MAX)
      return -1;
    newPayloadLen = NQ_WEBSOCKET_BUFFER_MAX;
    length = NQ_WEBSOCKET_BUFFER_MAX - thiz->payloadLen;
  }
  if (newPayloadLen > thiz->payloadCapacity) {
    if (!payloadCapacityResize(thiz, newPayloadLen))
      return -1;
  }

  memcpy(thiz->payloadPtr + thiz->payloadLen, data, length);
  thiz->payloadLen = newPayloadLen;
  return (int)length;
}

void NQWebSocketBuffer_complete(NQWebSocketBuffer* thiz, uint8_t opcode, bool fin, const uint8_t* mask)
{
  uint8_t flag1;
  uint8_t flag2;
  uint16_t len2;
  uint64_t len8;

  if (thiz->payloadPtr == NULL)
    return;

  flag1 = opcode;
  if (fin)
    flag1 |= WS_FIN_MASK;

  thiz->headerSize = sizeof(flag1) + sizeof(flag2);

  if (thiz->payloadLen < 126) {
    flag2 = (WS_LEN1_MASK & thiz->payloadLen);
    len2 = len8 = 0;
  }
  else if (thiz->payloadLen <= NQ_UINT16_MAX) {
    flag2 = (WS_LEN1_MASK & 126);
    len2 = (uint16_t)thiz->payloadLen;
    len8 = 0;
    thiz->headerSize += sizeof(len2);
  }
  else {
    flag2 = (WS_LEN1_MASK & 127);
    len2 = 0;
    len8 = (uint64_t)thiz->payloadLen;
    thiz->headerSize += sizeof(len2);
  }

  if (mask) {
    thiz->headerSize += 4;
    flag2 |= WS_MASK_MASK;
  }

  uint8_t* headerPtr = thiz->payloadPtr - thiz->headerSize;
  *headerPtr++ = flag1;
  *headerPtr++ = flag2;

  if (len2 != 0)
    headerPtr += NQPutUint16BE(headerPtr, len2);
  else if (len8 != 0)
    headerPtr += NQPutUint64BE(headerPtr, len8);

  if (mask) {
    (void)memcpy(headerPtr, mask, 4);
    NQWebSocketFramePayloadUnmask(mask, thiz->payloadPtr, thiz->payloadLen);
    NQ_ASSERT(headerPtr + 4 == thiz->payloadPtr);
  }
}

void NQWebSocketBuffer_reset(NQWebSocketBuffer* thiz)
{
  thiz->headerSize = 0;
  thiz->payloadLen = 0;
}
