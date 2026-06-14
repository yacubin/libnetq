/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/crypto/Secp256k1.h"

#ifdef NQCONFIG_USE_SECP256K1_SECP256K1

#include <secp256k1.h>
#include <secp256k1_ecdh.h>
#include <secp256k1_ellswift.h>

#include <libnetq/Random.h>
#include <libnetq/crypto/SecureErase.h>
#include <libnetq/Malloc.h>
#include <libnetq/crypto/SHA256.h>
#include <libnetq/Assert.h>
#include <libnetq/Log.h>

NQ_STATIC_ASSERT(NQ_ECSECKEY_SIZE == NQSHA256_DIGEST_SIZE, "");

NQSecp256k1Ctx* NQSecp256k1Create(void)
{
  secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_NONE);
  if (ctx == NULL)
    return NULL;

  uint8_t seed[32];
  if (NQGetCryptoRandom(seed, sizeof(seed)) != 0)
    NQ_LOGE("Failed to generate randomness");
  else if (!secp256k1_context_randomize(ctx, seed))
    NQ_LOGE("Unable to randomize context");
  else
    NQSecureErase(seed, sizeof(seed));

  return ctx;
}

void NQSecp256k1Destroy(NQSecp256k1Ctx* ctx)
{
  secp256k1_context_destroy(ctx);
}

bool NQSecp256k1SeckeyGenerate(NQSecp256k1Ctx* ctx, uint8_t* seckey)
{
  do {
    int rc = NQGetCryptoRandom(seckey, NQ_ECSECKEY_SIZE);
    if (rc != 0) {
      NQ_LOGE("Error generating random bytes");
      return false;
    }
  } while (!secp256k1_ec_seckey_verify(ctx, seckey));

  return true;
}

NQSecp256k1Pubkey* NQSecp256k1PubkeyCreate(const NQSecp256k1Ctx* ctx, const uint8_t* seckey)
{
  NQSecp256k1Pubkey* pubkey = (NQSecp256k1Pubkey*)NQMalloc(sizeof(struct secp256k1_pubkey));
  if (pubkey == NULL)
    return NULL;

  if (secp256k1_ec_pubkey_create(ctx, pubkey, seckey) != 1) {
    NQFree(pubkey);
    return NULL;
  }

  return pubkey;
}

NQSecp256k1Pubkey* NQSecp256k1PubkeyParse(const NQSecp256k1Ctx* ctx, const uint8_t* data, size_t size)
{
  NQSecp256k1Pubkey* pubkey = (NQSecp256k1Pubkey*)NQMalloc(sizeof(struct secp256k1_pubkey));
  if (pubkey == NULL)
    return NULL;

  if (secp256k1_ec_pubkey_parse(ctx, pubkey, data, size) != 1) {
    NQFree(pubkey);
    return NULL;
  }

  return pubkey;
}

void NQSecp256k1PubkeyDestroy(const NQSecp256k1Ctx* ctx, NQSecp256k1Pubkey* pubkey)
{
  NQFree(pubkey);
}

bool NQSecp256k1PubkeySerialize(const NQSecp256k1Ctx* ctx, const NQSecp256k1Pubkey* pubkey, uint8_t* data, size_t size)
{
  unsigned flags;

  if (size == NQ_ECPUBKEY_SIZE)
    flags = SECP256K1_EC_COMPRESSED;
  else if (size == NQ_ECPUBKEY_UNCOMPRESSED_SIZE)
    flags = SECP256K1_EC_UNCOMPRESSED;
  else {
    NQ_LOGE("Not supported %u size", (unsigned)size);
    return false;
  }

  size_t rsize = size;
  if (secp256k1_ec_pubkey_serialize(ctx, data, &rsize, pubkey, flags) != 1)
    return false;

  return (size == rsize);
}

bool NQSecp256k1EcdhPerform(const NQSecp256k1Ctx* ctx, const NQSecp256k1Pubkey* pubkey, const uint8_t* seckey, uint8_t* output32)
{
  return secp256k1_ecdh(ctx, output32, pubkey, seckey, NULL, NULL) == 1;
}

NQSecp256k1Signature* NQSecp256k1SignatureCreate(const NQSecp256k1Ctx* ctx, const uint8_t* seckey, const uint8_t* hash32)
{
  NQSecp256k1Signature* sig = (NQSecp256k1Signature*)NQMalloc(sizeof(struct secp256k1_ecdsa_signature));
  if (sig == NULL)
    return NULL;

  if (secp256k1_ecdsa_sign(ctx, sig, hash32, seckey, NULL, NULL) != 1) {
    NQFree(sig);
    return NULL;
  }

  return sig;
}

NQSecp256k1Signature* NQSecp256k1SignatureParse(const NQSecp256k1Ctx* ctx, const uint8_t* input64)
{
  NQSecp256k1Signature* sig = (NQSecp256k1Signature*)NQMalloc(sizeof(struct secp256k1_ecdsa_signature));
  if (sig == NULL)
    return NULL;

  if (secp256k1_ecdsa_signature_parse_compact(ctx, sig, input64) != 1) {
    NQFree(sig);
    return NULL;
  }

  return sig;
}

void NQSecp256k1SignatureDestroy(const NQSecp256k1Ctx* ctx, NQSecp256k1Signature* sig)
{
  NQFree(sig);
}

bool NQSecp256k1SignatureSerialize(const NQSecp256k1Ctx* ctx, const NQSecp256k1Signature* sig, uint8_t* output64)
{
  return secp256k1_ecdsa_signature_serialize_compact(ctx, output64, sig) == 1;
}

bool NQSecp256k1SignatureVerify(const NQSecp256k1Ctx* ctx, const NQSecp256k1Signature* sig, const NQSecp256k1Pubkey* pubkey, const uint8_t* hash32)
{
  if (ctx == NULL)
    ctx = secp256k1_context_static;
  return secp256k1_ecdsa_verify(ctx, sig, hash32, pubkey) == 1;
}

bool NQSecp256k1EllswiftMake(const NQSecp256k1Ctx* ctx, const uint8_t* seckey, const uint8_t* auxrnd32, uint8_t* ellspkey)
{
  return secp256k1_ellswift_create(ctx, ellspkey, seckey, auxrnd32) == 1;
}

bool NQSecp256k1EllswiftXdhA(const NQSecp256k1Ctx* ctx, const uint8_t* ellspkeyA, const uint8_t* ellspkeyB, const uint8_t* seckey, uint8_t* output32)
{
  return secp256k1_ellswift_xdh(ctx, output32, ellspkeyA, ellspkeyB, seckey, 0, secp256k1_ellswift_xdh_hash_function_bip324, NULL) == 1;
}

bool NQSecp256k1EllswiftXdhB(const NQSecp256k1Ctx* ctx, const uint8_t* ellspkeyA, const uint8_t* ellspkeyB, const uint8_t* seckey, uint8_t* output32)
{
  return secp256k1_ellswift_xdh(ctx, output32, ellspkeyA, ellspkeyB, seckey, 1, secp256k1_ellswift_xdh_hash_function_bip324, NULL) == 1;
}

#endif
