/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#define NQ_CLASS_NAME "NQSocket"
#define NQ_LOG_TAG NQ_CLASS_NAME

#include "config.h"
#include "libnetq/Socket.h"

#include <libnetq/CStrBase.h>
#include <libnetq/ObjectClass.h>
#include <libnetq/Malloc.h>
#include <libnetq/ErrorCode.h>
#include <libnetq/SocketHandle.h>
#include <libnetq/Looper.h>
#include <libnetq/LooperSource.h>
#include <libnetq/Assert.h>
#include <libnetq/Log.h>

extern const NQObjectClass __NQSocketClass;

enum State {
  MODE_NONE,
  MODE_CONNECT,
  MODE_CONNECTING,
  MODE_ERROR,
};

struct NQSocket {
  const NQObjectClass* class;

  NQLooper* looper;
  NQLooperSource looperSource;

  bool sync;
  void* userdata;
  NQStreamCallbacks callbacks;

  uint8_t mode;
  NQSocketHandle handle;
  uint8_t buffer[4096];
  NQErrorCode errorCode;

  NQEndPoint serverEndpoint;
  NQEndPoint clientEndpoint;
};

static void NQSocket_onEvent(void* userdata, const NQEvent* e)
{
}

static NQSocketHandle NQSocket_nativeHandle(void* userdata)
{
  NQSocket* thiz = (NQSocket*)userdata;
  return thiz->handle;
}

NQSocket* NQSocket_create(void* userdata, const NQStreamCallbacks* callbacks)
{
  NQSocket* thiz = (NQSocket*)NQMalloc(sizeof(NQSocket));
  if (thiz == NULL)
    return NULL;

  thiz->class = &__NQSocketClass;

  thiz->looperSource.type = NQ_SOURCE_SOCKET;
  thiz->looperSource.userdata = thiz;
  thiz->looperSource.handleEvent = NQSocket_onEvent;
  thiz->looperSource.u.getSocketHandle = NQSocket_nativeHandle;

  thiz->looper = NULL;

  if (callbacks == NULL) {
    thiz->sync = true;
    thiz->userdata = NULL;
    memset(&thiz->callbacks, 0, sizeof(thiz->callbacks));
  }
  else {
    thiz->sync = false;
    thiz->userdata = userdata;
    thiz->callbacks = *callbacks;
  }

  thiz->handle = NQ_INVALID_SOCKET;
  thiz->mode = MODE_NONE;
  thiz->errorCode = 0;

  return thiz;
}

void NQSocket_destroy(NQSocket* thiz)
{
  if (thiz->looper)
    NQSocket_detachLooper(thiz);

  if (thiz->handle != NQ_INVALID_SOCKET)
    NQSocketClose(thiz->handle);

  NQFree(thiz);
}

NQ_ALLOW_UNUSED
static int NQSocket_notifyError(NQSocket* thiz, int code, const char* msg)
{
  if (msg == NULL)
    msg = "";
  NQ_LOGE("code %i %s", code, msg);
  return code;
}

bool NQSocket_connect(NQSocket* thiz, const NQEndPoint* ep)
{
  if (ep == NULL)
    return false;

  if (thiz->mode != MODE_NONE) {
    NQ_LOGE("Mode does not match the connection");
    return false;
  }

  thiz->errorCode = NQSocketOpen(ep->family, NQ_SOCK_STREAM, 0, &thiz->handle);
  if (thiz->errorCode) {
    return false;
  }

  if (!thiz->sync) {
    if (thiz->looper == NULL) {
      if (!NQSocket_attachLooper(thiz, NQLooperGetMain())) {
        thiz->mode = MODE_ERROR;
        thiz->errorCode = -1;
        return false;
      }
    }
    if (!NQSocketSetBoolOpt(thiz->handle, NQ_SOCKOPT_NONBLOCK, true)) {
      thiz->mode = MODE_ERROR;
      thiz->errorCode = NQGetLastError();
      return false;
    }
  }

  thiz->clientEndpoint = *ep;
  if (NQSocketConnect(thiz->handle, &thiz->clientEndpoint)) {
    thiz->mode = MODE_CONNECT;
    return true;
  }

  int ec = NQGetLastError();
  if (!thiz->sync && (ec == NQ_EINPROGRESS || ec == NQ_EWOULDBLOCK)) {
    thiz->mode = MODE_CONNECTING;
    return true;
  }

  thiz->mode = MODE_ERROR;
  thiz->errorCode = ec;
  return false;
}

int NQSocket_send(NQSocket* thiz, const uint8_t* data, size_t size)
{
  if (thiz->mode != MODE_CONNECT) {
    NQ_LOGE("Mode does not match the sending");
    return -1;
  }

  int sz = NQSocketSend(thiz->handle, data, size, 0);
  if (sz < 0) {
    thiz->mode = MODE_ERROR;
    thiz->errorCode = NQGetLastError();
    return -1;
  }

  // TODO: copy buffer all not send data

  return sz;
}

int NQSocket_recv(NQSocket* thiz, uint8_t* data, size_t size)
{
  if (thiz->mode != MODE_CONNECT) {
    NQ_LOGE("Mode does not match the receive");
    return -1;
  }

  int sz = NQSocketRecv(thiz->handle, data, size, 0);
  if (sz < 0) {
    thiz->mode = MODE_ERROR;
    thiz->errorCode = NQGetLastError();
    return -1;
  }
  
  return sz;
}

void NQSocket_close(NQSocket* thiz)
{
  if (thiz->handle != NQ_INVALID_SOCKET) {
    NQSocketClose(thiz->handle);
    thiz->handle = NQ_INVALID_SOCKET;
    thiz->mode = MODE_NONE;
  }
}

bool NQSocket_attachLooper(NQSocket* thiz, NQLooper* looper)
{
  if (thiz->mode != MODE_NONE)
    return false;

  if (thiz->looper)
    return false;

  if (!NQLooper_attachSource(looper, &thiz->looperSource))
    return false;
  
  thiz->looper = looper;
  return true;
}

bool NQSocket_detachLooper(NQSocket* thiz)
{
  if (!thiz->looper)
    return false;

  if (!NQLooper_detachSource(thiz->looper, &thiz->looperSource))
    return false;

  thiz->looper = NULL;
  return true;
}

const NQObjectClass __NQSocketClass = {
  NQSocketObjectType,
  NQ_CLASS_NAME,
  NQ_VERSION_CODE,
  (NQObjectReleaseCallback)NQSocket_destroy,
};
