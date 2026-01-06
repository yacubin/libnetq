/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CRYPTO_SHA1STUB_H
#define _LIBNETQ_CRYPTO_SHA1STUB_H

#include <libnetq/Basic.h>

typedef struct NQSHA1 NQSHA1;
struct NQSHA1 {
  int dummy;
};

#define NQSHA1_DIGEST_SIZE 20
typedef uint8_t NQSHA1Digest[NQSHA1_DIGEST_SIZE];

static inline bool NQSHA1_init(NQSHA1* thiz)
{
  NQ_UNUSED_PARAM(thiz);
  return false;
}

static inline void NQSHA1_finalize(NQSHA1* thiz)
{
  NQ_UNUSED_PARAM(thiz);
}

static inline bool NQSHA1_update(NQSHA1* thiz, const uint8_t* data, size_t size)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(data);
  NQ_UNUSED_PARAM(size);
  return false;
}

static inline bool NQSHA1_final(NQSHA1* thiz, uint8_t* digest)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(digest);
  return false;
}

#endif /* _LIBNETQ_CRYPTO_SHA1STUB_H */
