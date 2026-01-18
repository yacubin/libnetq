/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CRYPTO_SHA1ADVAPI32_H
#define _LIBNETQ_CRYPTO_SHA1ADVAPI32_H

#include <libnetq/Basic.h>

#include <windows.h>
#include <wincrypt.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQSHA1 NQSHA1;
struct NQSHA1 {
  HCRYPTPROV hProv;
  HCRYPTHASH hHash;
};

#define NQSHA1_DIGEST_SIZE 20
typedef uint8_t NQSHA1Digest[NQSHA1_DIGEST_SIZE];

NQ_EXPORT bool NQSHA1_init(NQSHA1*);
NQ_EXPORT void NQSHA1_finalize(NQSHA1*);
NQ_EXPORT bool NQSHA1_update(NQSHA1*, const uint8_t* data, size_t size);
NQ_EXPORT bool NQSHA1_final(NQSHA1*, uint8_t* digest);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _LIBNETQ_CRYPTO_SHA1ADVAPI32_H */
