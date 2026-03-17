/*
 * MIT License
 *
 * Copyright (c) 2023-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_WEB_WEBSOCKET_H
#define _LIBNETQ_WEB_WEBSOCKET_H

#include <libnetq/NetworkLooper.h>

#ifdef __cplusplus
extern "C" {
#endif

#define WEB_WSOPCODE_CONTINUATION     0
#define WEB_WSOPCODE_TEXT             1
#define WEB_WSOPCODE_BINARY           2
#define WEB_WSOPCODE_CONNECTION_CLOSE 8
#define WEB_WSOPCODE_PING             9
#define WEB_WSOPCODE_PONG             10

#define WEB_WSBIT_FIN                 16

typedef struct NQWebSocket NQWebSocket;

struct NQWebSocketOperations {
  const char* name;
  int  (*send)    (NQWebSocket*, const uint8_t* data, size_t size, unsigned flags);
  void (*close)   (NQWebSocket*, uint16_t statusCode);
  void (*release) (NQWebSocket*);
};

struct NQWebSocket {
  const struct NQWebSocketOperations* operations;

  void* userdata;
  NQNetworkLooper* looper;

  void (*onOpen)    (NQWebSocket*);
  void (*onReceive) (NQWebSocket*, const uint8_t* data, size_t size, unsigned opcode);
  void (*onClose)   (NQWebSocket*, uint32_t reason);
};

enum {
  WEB_WSRES_SUCCESS = 0,
  WEB_WSRES_UNKNOWN,
  WEB_WSRES_HANG_UP,
  WEB_WSRES_NOT_OPEN,
  WEB_WSRES_POLL,
  WEB_WSRES_UNKNOWN_FRAME,
  WEB_WSRES_SEND,
  WEB_WSRES_NOMEMORY,
};

static inline void NQWebSocket_release(NQWebSocket* thiz)
{
  thiz->operations->release(thiz);
}

static inline int NQWebSocket_send(NQWebSocket* thiz, const uint8_t* data, size_t size, unsigned flags)
{
  return thiz->operations->send(thiz, data, size, flags);
}

static inline void NQWebSocket_close(NQWebSocket* thiz, uint16_t statusCode)
{
  thiz->operations->close(thiz, statusCode);
}

static inline void* NQWebSocket_userdata(const NQWebSocket* thiz)
{
  return thiz->userdata;
}

static inline void NQWebSocket_setUserdata(NQWebSocket* thiz, void* userdata)
{
  thiz->userdata = userdata;
}

static inline NQNetworkLooper* NQWebSocket_looper(NQWebSocket* thiz)
{
  return thiz->looper;
}

static inline void NQWebSocket_doOpen(NQWebSocket* thiz)
{
  if (thiz->onOpen)
    thiz->onOpen(thiz);
}

static inline void NQWebSocket_doReceive(NQWebSocket* thiz, const uint8_t* data, size_t size, unsigned opcode)
{
  if (thiz->onReceive)
    thiz->onReceive(thiz, data, size, opcode);
}

static inline void NQWebSocket_doClose(NQWebSocket* thiz, uint32_t reason)
{
  if (thiz->onClose)
    thiz->onClose(thiz, reason);
}

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_WEB_WEBSOCKET_H */
