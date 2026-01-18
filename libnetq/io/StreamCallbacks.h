/*
 * MIT License
 *
 * Copyright (c) 2021-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_IO_STREAMCALLBACKS_H
#define _LIBNETQ_IO_STREAMCALLBACKS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQStreamCallbacks NQStreamCallbacks;

struct NQStreamCallbacks {
  void (*onReceive) (void* userdata, const uint8_t* data, size_t size);
  void (*onClose)   (void* userdata);
  void (*onError)   (void* userdata, int code);
};

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_IO_STREAMCALLBACKS_H */
