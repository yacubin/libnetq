/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_IO_NSVARWRITER_H
#define _LIBNETQ_IO_NSVARWRITER_H

#include <libnetq/Basic.h>
#include <libnetq/io/WriteWrapper.h>

#ifdef __cplusplus
extern "C" {
#endif

// ${ENV:NAME}
// ${FILE:PATH}

struct NQNsVarWriterEntry {
  const char* name;
  int (*handler) (const char* value, NQWriteCallback writer, void* userdata);
};

typedef struct NQNsVarWriter NQNsVarWriter;
struct NQNsVarWriter {
  NQWriteWrapper base;
  const struct NQNsVarWriterEntry* entries;
  const struct NQNsVarWriterEntry* current;
  char separator;
  uint8_t state;
  uint16_t bufferSize;
  uint16_t valueIndex;
  char buffer[128];
};

NQ_EXPORT void NQNsVarWriter_init(NQNsVarWriter*, const struct NQNsVarWriterEntry* entries, NQWriteCallback writer, void* userdata);
static inline void NQNsVarWriter_finalize(NQNsVarWriter* thiz)
{
  NQ_UNUSED_PARAM(thiz);
}
NQ_EXPORT int NQNsVarWriter_write(NQNsVarWriter* thiz, const void* data, size_t size);
NQ_EXPORT int NQNsVarWriter_flush(NQNsVarWriter*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_IO_NSVARWRITER_H */
