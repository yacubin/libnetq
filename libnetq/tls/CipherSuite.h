/*
 * MIT License
 *
 * Copyright (c) 2021-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_TLS_CIPHERSUITE_H
#define _LIBNETQ_TLS_CIPHERSUITE_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint16_t NQCipherSuite;

#define NQ_TLS_AES_128_GCM_SHA256 0x1301
#define NQ_TLS_AES_256_GCM_SHA384 0x1302
#define NQ_TLS_CHACHA20_POLY1305_SHA256 0x1303
#define NQ_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256 0xC02B
#define NQ_TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256 0xC02F
#define NQ_TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384 0xC02C
#define NQ_TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384 0xC030
#define NQ_TLS_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256 0xCCA9
#define NQ_TLS_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256 0xCCA8
#define NQ_TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA 0xC013
#define NQ_TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA 0xC014
#define NQ_TLS_RSA_WITH_AES_128_GCM_SHA256 0x009C
#define NQ_TLS_RSA_WITH_AES_256_GCM_SHA384 0x009D
#define NQ_TLS_RSA_WITH_AES_128_CBC_SHA 0x002F
#define NQ_TLS_RSA_WITH_AES_256_CBC_SHA 0x0035

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_TLS_CIPHERSUITE_H */
