/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CRYPTO_MD5STUB_H
#define _LIBNETQ_CRYPTO_MD5STUB_H

#include <libnetq/Basic.h>

typedef struct NQMD5 NQMD5;
struct NQMD5 {
  int dummy;
};

#define NQMD5_DIGEST_SIZE 16
typedef uint8_t NQMD5Digest[NQMD5_DIGEST_SIZE];

static inline bool NQMD5_init(NQMD5* thiz)
{
  NQ_UNUSED_PARAM(thiz);
  return false;
}

static inline void NQMD5_finalize(NQMD5* thiz)
{
  NQ_UNUSED_PARAM(thiz);
}

static inline bool NQMD5_update(NQMD5* thiz, const uint8_t* data, size_t size)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(data);
  NQ_UNUSED_PARAM(size);
  return false;
}

static inline bool NQMD5_final(NQMD5* thiz, uint8_t digest[NQMD5_DIGEST_SIZE])
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(digest);
  return false;
}

#endif /* _LIBNETQ_CRYPTO_MD5STUB_H */
