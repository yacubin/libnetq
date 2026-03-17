/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CRYPTO_SECP256K1_H
#define _LIBNETQ_CRYPTO_SECP256K1_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct secp256k1_context_struct NQSecp256k1Ctx;
typedef struct secp256k1_pubkey NQSecp256k1Pubkey;
typedef struct secp256k1_ecdsa_signature NQSecp256k1Signature;

#define NQ_ECSECKEY_SIZE 32
#define NQ_ECPUBKEY_SIZE 33
#define NQ_ECPUBKEY_UNCOMPRESSED_SIZE 65
#define NQ_ECSIGNATURE_SIZE 64
#define NQ_ECELLSPKEY_SIZE 64

NQ_EXPORT NQSecp256k1Ctx* NQSecp256k1Create(void);
NQ_EXPORT void NQSecp256k1Destroy(NQSecp256k1Ctx*);

NQ_EXPORT bool NQSecp256k1SeckeyGenerate(NQSecp256k1Ctx*, uint8_t* seckey);

NQ_EXPORT NQSecp256k1Pubkey* NQSecp256k1PubkeyCreate(const NQSecp256k1Ctx*, const uint8_t* seckey);
NQ_EXPORT NQSecp256k1Pubkey* NQSecp256k1PubkeyParse(const NQSecp256k1Ctx*, const uint8_t* data, size_t size);
NQ_EXPORT void NQSecp256k1PubkeyDestroy(const NQSecp256k1Ctx*, NQSecp256k1Pubkey* pubkey);

NQ_EXPORT bool NQSecp256k1PubkeySerialize(const NQSecp256k1Ctx*, const NQSecp256k1Pubkey* pubkey, uint8_t* data, size_t size);
NQ_EXPORT bool NQSecp256k1EcdhPerform(const NQSecp256k1Ctx*, const NQSecp256k1Pubkey* pubkey, const uint8_t* seckey, uint8_t* output32);

NQ_EXPORT NQSecp256k1Signature* NQSecp256k1SignatureCreate(const NQSecp256k1Ctx* ctx, const uint8_t* seckey, const uint8_t* hash32);
NQ_EXPORT NQSecp256k1Signature* NQSecp256k1SignatureParse(const NQSecp256k1Ctx*, const uint8_t* input64);
NQ_EXPORT void NQSecp256k1SignatureDestroy(const NQSecp256k1Ctx* ctx, NQSecp256k1Signature* sig);

NQ_EXPORT bool NQSecp256k1SignatureSerialize(const NQSecp256k1Ctx*, const NQSecp256k1Signature* sig, uint8_t* output64);
NQ_EXPORT bool NQSecp256k1SignatureVerify(const NQSecp256k1Ctx*, const NQSecp256k1Signature* sig, const NQSecp256k1Pubkey* pubkey, const uint8_t* hash32);

NQ_EXPORT bool NQSecp256k1EllswiftMake(const NQSecp256k1Ctx*, const uint8_t* seckey, const uint8_t* auxrnd32, uint8_t* ellspkey);
NQ_EXPORT bool NQSecp256k1EllswiftXdhA(const NQSecp256k1Ctx*, const uint8_t* ellspkeyA, const uint8_t* ellspkeyB, const uint8_t* seckey, uint8_t* output32);
NQ_EXPORT bool NQSecp256k1EllswiftXdhB(const NQSecp256k1Ctx*, const uint8_t* ellspkeyA, const uint8_t* ellspkeyB, const uint8_t* seckey, uint8_t* output32);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _LIBNETQ_CRYPTO_SECP256K1_H */
