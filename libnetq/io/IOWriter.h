/*
 * MIT License
 *
 * Copyright (c) 2025-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_IO_IOWRITER_H
#define _LIBNETQ_IO_IOWRITER_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQIOWriter NQIOWriter;

struct NQIOWriterCallbacks {
  int (*write) (NQIOWriter*, const void* data, size_t size);
  int (*flush) (NQIOWriter*);
  void (*release) (NQIOWriter*);
};

struct NQIOWriter {
  const struct NQIOWriterCallbacks* callbacks;
};

NQ_EXPORT NQIOWriter* NQIOWriter_fromFile(const char* filename);

static inline int NQIOWriter_write(NQIOWriter* thiz, const void* data, size_t size)
{
  return thiz->callbacks->write(thiz, data, size);
}

NQ_EXPORT bool NQIOWriter_writeAll(NQIOWriter*, const void* data, size_t size);

static inline int NQIOWriter_flush(NQIOWriter* thiz)
{
  return thiz->callbacks->flush(thiz);
}

static inline void NQIOWriter_release(NQIOWriter* thiz)
{
  thiz->callbacks->release(thiz);
}

NQ_EXPORT int NQIOWriter_flushStub(NQIOWriter*);
NQ_EXPORT void NQIOWriter_releaseStub(NQIOWriter*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_IO_IOWRITER_H */
