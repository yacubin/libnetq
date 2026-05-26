/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_IO_READWRAPPER_H
#define _LIBNETQ_IO_READWRAPPER_H

#include <libnetq/Endian.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*NQReadCallback) (void* userdata, void* data, size_t size);

typedef struct NQReadWrapper NQReadWrapper;
struct NQReadWrapper {
  NQReadCallback read;
  void* userdata;
  uint64_t numberOfBytesRead;
};

static inline void NQReadWrapper_init(NQReadWrapper* thiz, NQReadCallback read, void* userdata)
{
  thiz->read = read;
  thiz->userdata = userdata;
  thiz->numberOfBytesRead = 0;
}

static inline int NQReadWrapper_read(NQReadWrapper* thiz, void* data, size_t size)
{
  int ret = thiz->read(thiz->userdata, data, size);
  if (ret > 0)
    thiz->numberOfBytesRead += ret;
  return ret;
}

NQ_EXPORT bool NQReadWrapper_readAll(NQReadWrapper*, void* data, size_t size);

static inline bool NQReadWrapper_readUint8(NQReadWrapper* thiz, uint8_t* value)
{
  return NQReadWrapper_readAll(thiz, value, sizeof(*value));
}

static inline bool NQReadWrapper_readUint32LE(NQReadWrapper* thiz, uint32_t* value)
{
  uint32_t temp;
  if (!NQReadWrapper_readAll(thiz, &temp, sizeof(temp)))
    return false;
  *value = NQHostToLE32(temp);
  return true;
}

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_IO_READWRAPPER_H */
