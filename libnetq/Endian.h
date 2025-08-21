/*
 * MIT License
 *
 * Copyright (c) 2021-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_ENDIAN_H
#define _LIBNETQ_ENDIAN_H

#include <libnetq/ByteOrder.h>
#include <libnetq/CPU.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(NQ_CPU_BIG_ENDIAN)
#define NQNetworkToHost16(x) (x)
#define NQHostToNetwork16(x) (x)
#define NQNetworkToHost32(x) (x)
#define NQHostToNetwork32(x) (x)

#define NQHostToLE16(x) NQByteSwap16(x)
#define NQLEToHost16(x) NQByteSwap16(x)
#define NQHostToBE16(x) (x)
#define NQBEToHost16(x) (x)
#define NQHostToLE32(x) NQByteSwap32(x)
#define NQLEToHost32(x) NQByteSwap32(x)
#define NQHostToBE32(x) (x)
#define NQBEToHost32(x) (x)
#define NQHostToLE64(x) NQByteSwap64(x)
#define NQLEToHost64(x) NQByteSwap64(x)
#define NQHostToBE64(x) (x)
#define NQBEToHost64(x) (x)

#elif defined(NQ_CPU_MIDDLE_ENDIAN)
#define NQNetworkToHost16(x) (x)
#define NQHostToNetwork16(x) (x)
#define NQNetworkToHost32(x) NQWordSwap32(x)
#define NQHostToNetwork32(x) NQWordSwap32(x)

// TODO:
#define NQHostToLE16(x) (x)
#define NQLEToHost16(x) (x)
#define NQHostToBE16(x) (x)
#define NQBEToHost16(x) (x)
#define NQHostToLE32(x) (x)
#define NQLEToHost32(x) (x)
#define NQHostToBE32(x) (x)
#define NQBEToHost32(x) (x)
#define NQHostToLE64(x) (x)
#define NQLEToHost64(x) (x)
#define NQHostToBE64(x) (x)
#define NQBEToHost64(x) (x)

#else
#define NQNetworkToHost16(x) NQByteSwap16(x)
#define NQHostToNetwork16(x) NQByteSwap16(x)
#define NQNetworkToHost32(x) NQByteSwap32(x)
#define NQHostToNetwork32(x) NQByteSwap32(x)

#define NQHostToLE16(x) (x)
#define NQLEToHost16(x) (x)
#define NQHostToBE16(x) NQByteSwap16(x)
#define NQBEToHost16(x) NQByteSwap16(x)
#define NQHostToLE32(x) (x)
#define NQLEToHost32(x) (x)
#define NQHostToBE32(x) NQByteSwap32(x)
#define NQBEToHost32(x) NQByteSwap32(x)
#define NQHostToLE64(x) (x)
#define NQLEToHost64(x) (x)
#define NQHostToBE64(x) NQByteSwap64(x)
#define NQBEToHost64(x) NQByteSwap64(x)
#endif

NQ_EXPORT bool NQIsLittleEndian();
NQ_EXPORT bool NQIsBigEndian();

#define nq_bswap16(x) NQByteSwap16(x)
#define nq_bswap32(x) NQByteSwap32(x)
#define nq_bswap64(x) NQByteSwap64(x)
#define nq_wswap32(x) NQWordSwap32(x)
#define nq_wswap64(x) NQWordSwap64(x)
#define nq_dswap64(x) NQDwordSwap64(x)

#define nq_ntohs(x) NQNetworkToHost16(x)
#define nq_htons(x) NQHostToNetwork16(x)
#define nq_ntohl(x) NQNetworkToHost32(x)
#define nq_htonl(x) NQHostToNetwork32(x)

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_ENDIAN_H */
