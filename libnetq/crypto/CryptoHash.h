/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CRYPTO_CRYPTOHASH_H
#define _LIBNETQ_CRYPTO_CRYPTOHASH_H

#include <libnetq/Basic.h>
#include <libnetq/crypto/CryptoAlgorithm.h>

#if defined(NQCONFIG_USE_OPENSSL_CRYPTOHASH)
# include <libnetq/crypto/openssl/CryptoHash.h>
#elif defined(NQCONFIG_USE_ADVAPI32_CRYPTOHASH)
# include <libnetq/crypto/advapi32/CryptoHash.h>
#elif defined(NQ_USE_KERNEL_CRYPTOHASH)
# include <libnetq/crypto/kernel/CryptoHash.h>
#elif defined(NQCONFIG_USE_STUB_CRYPTOHASH)
# include <libnetq/crypto/stub/CryptoHash.h>
#else
# error There is no implementation for the CryptoHash
#endif

#endif /* _LIBNETQ_CRYPTO_CRYPTOHASH_H */
