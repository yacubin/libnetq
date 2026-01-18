/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CRYPTO_HMAC_SHA1_H
#define _LIBNETQ_CRYPTO_HMAC_SHA1_H

#include <libnetq/crypto/SHA1.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQHMAC_SHA1 NQHMAC_SHA1;
struct NQHMAC_SHA1 {
  NQSHA1 opad;
  NQSHA1 ipad;
};

NQ_EXPORT bool NQHMAC_SHA1_init(NQHMAC_SHA1*, const uint8_t* key, size_t length);

static inline void NQHMAC_SHA1_finalize(NQHMAC_SHA1* thiz)
{
  NQSHA1_finalize(&thiz->opad);
  NQSHA1_finalize(&thiz->ipad);
}

static inline bool NQHMAC_SHA1_update(NQHMAC_SHA1* thiz, const uint8_t* data, size_t size)
{
  return NQSHA1_update(&thiz->ipad, data, size);
}

NQ_EXPORT bool NQHMAC_SHA1_final(NQHMAC_SHA1*, uint8_t digest[NQSHA1_DIGEST_SIZE]);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _LIBNETQ_CRYPTO_HMAC_SHA1_H */
