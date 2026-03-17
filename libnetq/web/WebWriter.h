/*
 * MIT License
 *
 * Copyright (c) 2023-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_WEB_WRITER_H
#define _LIBNETQ_WEB_WRITER_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQWebRequest NQWebRequest;
typedef struct NQWebWriter NQWebWriter;

struct NQWebWriterOperations {
  int (*init)    (NQWebWriter*);
  int (*write)   (NQWebWriter*, const void* data, size_t size);
  int (*flush)   (NQWebWriter*);
  int (*release) (NQWebWriter*);
};

struct NQWebWriter {
  const struct NQWebWriterOperations* operations;
  NQWebWriter* next;
  NQWebRequest* request;
  void* userdata;
};

static inline int NQWebWriter_init(NQWebWriter* thiz)
{
  return thiz->operations->init(thiz);
}

static inline int NQWebWriter_write(NQWebWriter* thiz, const void* data, size_t size)
{
  return thiz->operations->write(thiz, data, size);
}

static inline int NQWebWriter_flush(NQWebWriter* thiz)
{
  return thiz->operations->flush(thiz);
}

static inline int NQWebWriter_release(NQWebWriter* thiz)
{
  return thiz->operations->release(thiz);
}

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_WEB_WRITER_H */
