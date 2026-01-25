/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CRYPTO_SHA1_H
#define _LIBNETQ_CRYPTO_SHA1_H

#include <libnetq/Basic.h>

#ifdef NQ_SYS_LINUX
# include <libnetq/crypto/SHA1SysLinux.h>
#elif USE_OPENSSL_SHA1
# include <libnetq/crypto/SHA1OpenSSL.h>
#elif USE_ADVAPI32_SHA1
# include <libnetq/crypto/SHA1Advapi32.h>
#else
# include <libnetq/crypto/SHA1Stub.h>
#endif

#endif /* _LIBNETQ_CRYPTO_SHA1_H */
