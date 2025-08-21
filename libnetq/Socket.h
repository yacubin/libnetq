/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_SOCKET_H
#define _LIBNETQ_SOCKET_H

#include <libnetq/Network.h>
#include <libnetq/StreamCallbacks.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQSocket NQSocket;
typedef struct NQLooper NQLooper;

NQ_EXPORT NQSocket* NQSocket_create(void* userdata, const NQStreamCallbacks* callbacks);
NQ_EXPORT void NQSocket_destroy(NQSocket*);

NQ_EXPORT bool NQSocket_connect(NQSocket*, const NQEndPoint* ep);
NQ_EXPORT int NQSocket_send(NQSocket*, const uint8_t* data, size_t size);
NQ_EXPORT int NQSocket_recv(NQSocket*, uint8_t* data, size_t size);
NQ_EXPORT void NQSocket_close(NQSocket*);

NQ_EXPORT bool NQSocket_attachLooper(NQSocket*, NQLooper* looper);
NQ_EXPORT bool NQSocket_detachLooper(NQSocket*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_SOCKET_H */
