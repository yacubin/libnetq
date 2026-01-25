/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CRYPTO_MD5_H
#define _LIBNETQ_CRYPTO_MD5_H

#include <libnetq/Basic.h>

#ifdef NQ_SYS_LINUX
# include <libnetq/crypto/MD5SysLinux.h>
#elif USE_OPENSSL_MD5
# include <libnetq/crypto/MD5OpenSSL.h>
#elif USE_ADVAPI32_MD5
# include <libnetq/crypto/MD5Advapi32.h>
#else
# include <libnetq/crypto/MD5Stub.h>
#endif

#endif /* _LIBNETQ_CRYPTO_MD5_H */
