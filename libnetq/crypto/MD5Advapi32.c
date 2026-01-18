/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/crypto/MD5.h"

#if USE_ADVAPI32_MD5

#include <libnetq/Log.h>

bool NQMD5_init(NQMD5* thiz)
{
  NQMD5 md5;

  if (!CryptAcquireContext(&md5.hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
    NQ_LOGE("CryptAcquireContext failed: %d", GetLastError());
    return false;
  }
  if (!CryptCreateHash(md5.hProv, CALG_MD5, 0, 0, &md5.hHash)) {
    NQ_LOGE("CryptCreateHash failed: %d", GetLastError());
    CryptReleaseContext(md5.hProv, 0);
    return false;
  }
  *thiz = md5;
  return true;
}

void NQMD5_finalize(NQMD5* thiz)
{
  CryptReleaseContext(thiz->hProv, 0);
  CryptDestroyHash(thiz->hHash);
}

bool NQMD5_update(NQMD5* thiz, const uint8_t* data, size_t size)
{
  if (!CryptHashData(thiz->hHash, data, size, 0)) {
    NQ_LOGE("CryptHashData failed: %d", GetLastError());
    return false;
  }
  return true;
}

bool NQMD5_final(NQMD5* thiz, uint8_t digest[NQMD5_DIGEST_SIZE])
{
  DWORD cbHash = NQMD5_DIGEST_SIZE;
  if (!CryptGetHashParam(thiz->hHash, HP_HASHVAL, digest, &cbHash, 0)) {
    NQ_LOGE("CryptGetHashParam failed: %d", GetLastError());
    return false;
  }
  return true;
}

#endif /* USE_WINCRYPT_MD5 */
