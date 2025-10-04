/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_NETWORK_H
#define _LIBNETQ_NETWORK_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQIPv4Address {
  uint8_t data[4];
} NQIPv4Address;

NQ_EXPORT void NQIPv4Address_init(NQIPv4Address* ip4, uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4);
NQ_EXPORT int NQIPv4Address_sprintf(const NQIPv4Address* ip4, char* s, size_t n);
NQ_EXPORT bool NQIPv4Address_parse(NQIPv4Address* ip4, const char* s);
NQ_EXPORT bool NQIPv4Address_parseWithLength(NQIPv4Address* ip4, const char* s, size_t len);
NQ_EXPORT bool NQIPv4Address_isBroadcast(const NQIPv4Address* ip4);
NQ_EXPORT bool NQIPv4Address_isAny(const NQIPv4Address* ip4);
NQ_EXPORT bool NQIPv4Address_isUnicast(const NQIPv4Address* ip4);

NQ_EXPORT const NQIPv4Address* NQIPv4Address_any(void);
NQ_EXPORT const NQIPv4Address* NQIPv4Address_loopback(void);
NQ_EXPORT const NQIPv4Address* NQIPv4Address_broadcast(void);


typedef struct NQIPv4EndPoint {
  uint16_t port;
  NQIPv4Address address;
} NQIPv4EndPoint;

NQ_EXPORT void NQIPv4EndPoint_init(NQIPv4EndPoint* ep4, const NQIPv4Address* address, uint16_t port);
NQ_EXPORT int NQIPv4EndPoint_sprintf(const NQIPv4EndPoint* ep4, char* s, size_t n);
NQ_EXPORT bool NQIPv4EndPoint_parse(NQIPv4EndPoint* ep4, const char* s);
NQ_EXPORT bool NQIPv4EndPoint_parseWithLength(NQIPv4EndPoint* ep4, const char* s, size_t len);


typedef struct NQIPv6Address {
  uint8_t data[16];
} NQIPv6Address;

typedef struct NQIPv6EndPoint {
  uint16_t port;
  NQIPv6Address address;
} NQIPv6EndPoint;

typedef uint16_t NQAddressFamily;
enum {
  NQ_AF_NONE,
  NQ_AF_INET4,
  NQ_AF_INET6,
};

typedef struct NQEndPoint {
  NQAddressFamily family;
  union {
    NQIPv4EndPoint ip4ep;
    NQIPv6EndPoint ip6ep;
  };
} NQEndPoint;

typedef struct NQMACAddress {
  uint8_t data[6];
} NQMACAddress;

NQ_EXPORT void NQEndPoint_init4(NQEndPoint* ep, const uint8_t data[4], uint16_t port);
NQ_EXPORT void NQEndPoint_init6(NQEndPoint* ep, const uint8_t data[16], uint16_t port);
NQ_EXPORT int NQEndPoint_sprintf(const NQEndPoint* ep, char* s, size_t n);
NQ_EXPORT bool NQEndPoint_parse(NQEndPoint* ep, const char* s);
NQ_EXPORT bool NQEndPoint_parseWithLength(NQEndPoint* ep, const char* s, size_t len);

NQ_EXPORT void NQMACAddress_reset(NQMACAddress* mac);
NQ_EXPORT bool NQMACAddress_parse(NQMACAddress* mac, const char* s, size_t n);
NQ_EXPORT int NQMACAddress_sprintf(const NQMACAddress* mac, char* s, size_t n);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_NETWORK_H */
