/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/crypto/CryptoHash.h"

#ifdef NQ_USE_ADVAPI32_CRYPTOHASH

#include <libnetq/Log.h>

bool NQCryptoHash_init(NQCryptoHash* thiz, uint32_t algId)
{
  NQCryptoHash self;

  ALG_ID platformAlgId;
  switch (algId) {
  case NQ_CRYPTOALGORITHM_MD4:
    platformAlgId = CALG_MD4;
    break;
  case NQ_CRYPTOALGORITHM_MD5:
    platformAlgId = CALG_MD5;
    break;
  case NQ_CRYPTOALGORITHM_SHA1:
    platformAlgId = CALG_SHA1;
    break;
  case NQ_CRYPTOALGORITHM_SHA256:
    platformAlgId = CALG_SHA_256;
    break;
  default:
    NQ_LOGE("Unknown crypt algorithm (%u)", algId);
    return false;
  }

  if (!CryptAcquireContext(&self.hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
    NQ_LOGE("CryptAcquireContext failed: %u", (unsigned)GetLastError());
    return false;
  }
  if (!CryptCreateHash(self.hProv, platformAlgId, 0, 0, &self.hHash)) {
    NQ_LOGE("CryptCreateHash failed: %u", (unsigned)GetLastError());
    CryptReleaseContext(self.hProv, 0);
    return false;
  }
  DWORD hashSizeSize = sizeof(self.hashSize);
  if (!CryptGetHashParam(self.hProv, HP_HASHSIZE, (BYTE*)&self.hashSize, &hashSizeSize, 0)) {
    NQ_LOGE("CryptGetHashParam failed: %u", (unsigned)GetLastError());
    CryptReleaseContext(self.hProv, 0);
    return false;
  }
  *thiz = self;
  return true;
}

void NQCryptoHash_finalize(NQCryptoHash* thiz)
{
  CryptReleaseContext(thiz->hProv, 0);
  CryptDestroyHash(thiz->hHash);
}

bool NQCryptoHash_update(NQCryptoHash* thiz, const uint8_t* data, size_t size)
{
  if (!CryptHashData(thiz->hHash, data, (DWORD)size, 0)) {
    NQ_LOGE("CryptHashData failed: %u", (unsigned)GetLastError());
    return false;
  }
  return true;
}

bool NQCryptoHash_final(NQCryptoHash* thiz, uint8_t* digest)
{
  DWORD rsize = thiz->hashSize;
  if (!CryptGetHashParam(thiz->hHash, HP_HASHVAL, digest, &rsize, 0)) {
    NQ_LOGE("CryptGetHashParam failed: %u", (unsigned)GetLastError());
    return false;
  }
  return true;
}

#endif /* NQ_USE_ADVAPI32_CRYPTOHASH */
