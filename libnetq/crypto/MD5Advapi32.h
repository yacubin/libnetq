/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CRYPTO_MD5ADVAPI32_H
#define _LIBNETQ_CRYPTO_MD5ADVAPI32_H

#include <libnetq/Basic.h>

#include <windows.h>
#include <wincrypt.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQMD5 NQMD5;
struct NQMD5 {
  HCRYPTPROV hProv;
  HCRYPTHASH hHash;
};

#define NQMD5_DIGEST_SIZE 16
typedef uint8_t NQMD5Digest[NQMD5_DIGEST_SIZE];

NQ_EXPORT bool NQMD5_init(NQMD5*);
NQ_EXPORT void NQMD5_finalize(NQMD5*);
NQ_EXPORT bool NQMD5_update(NQMD5*, const uint8_t* data, size_t size);
NQ_EXPORT bool NQMD5_final(NQMD5*, uint8_t digest[NQMD5_DIGEST_SIZE]);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _LIBNETQ_CRYPTO_MD5ADVAPI32_H */
