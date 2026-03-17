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

#if defined(NQ_USE_OPENSSL_CRYPTOHASH)
# include <libnetq/crypto/CryptoHashOpenSSL.h>
#elif defined(NQ_USE_ADVAPI32_CRYPTOHASH)
# include <libnetq/crypto/CryptoHashAdvapi32.h>
#elif defined(NQ_USE_STUB_CRYPTOHASH)
# include <libnetq/crypto/CryptoHashStub.h>
#elif defined(NQ_SYS_LINUX)
# include <libnetq/crypto/CryptoHashSysLinux.h>
#else
#error There is no implementation for the CryptoHash
#endif

#endif /* _LIBNETQ_CRYPTO_CRYPTOHASH_H */
