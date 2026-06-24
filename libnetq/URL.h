/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_URL_H
#define _LIBNETQ_URL_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQURL NQURL;

#define NQ_URL_MAX 2048

#define NQ_DEFAULT_FTP_PORT      21
#define NQ_DEFAULT_TFTP_PORT     69
#define NQ_DEFAULT_HTTP_PORT     80
#define NQ_DEFAULT_DNS_PORT      53
#define NQ_DEFAULT_HTTPS_PORT    443

#define NQ_URL_ABOUT_SCHEME      "about"
#define NQ_URL_BLOB_SCHEME       "blob"
#define NQ_URL_DATA_SCHEME       "data"
#define NQ_URL_FILE_SCHEME       "file"
#define NQ_URL_FILESYSTEM_SCHEME "filesystem"
#define NQ_URL_FTP_SCHEME        "ftp"
#define NQ_URL_GEO_SCHEME        "geo"
#define NQ_URL_GOPHER_SCHEME     "gopher"
#define NQ_URL_HTTP_SCHEME       "http"
#define NQ_URL_HTTPS_SCHEME      "https"
#define NQ_URL_IMAP_SCHEME       "imap"
#define NQ_URL_IRC_SCHEME        "irc"
#define NQ_URL_JAVASCRIPT_SCHEME "javascript"
#define NQ_URL_JDBC_SCHEME       "jdbc"
#define NQ_URL_LDAP_SCHEME       "ldap"
#define NQ_URL_MAGNET_SCHEME     "magnet"
#define NQ_URL_MAILTO_SCHEME     "mailto"
#define NQ_URL_MAN_SCHEME        "man"
#define NQ_URL_NNTP_SCHEME       "nntp"
#define NQ_URL_RTSP_SCHEME       "rtsp"
#define NQ_URL_SIP_SCHEME        "sip"
#define NQ_URL_SIPS_SCHEME       "sips"
#define NQ_URL_SMS_SCHEME        "sms"
#define NQ_URL_SSH_SCHEME        "ssh"
#define NQ_URL_TEL_SCHEME        "tel"
#define NQ_URL_TFTP_SCHEME       "tftp"
#define NQ_URL_UNIX_SCHEME       "unix"
#define NQ_URL_WS_SCHEME         "ws"
#define NQ_URL_WSS_SCHEME        "wss"

#define NQ_URL_SCHEME_SEPARATOR  "://"
#define NQ_URL_DELIMITER '/'
#define NQ_URL_PLUS_AS_SPACE (1)

enum NQURLCharType {
  kNQURLCharUnreserved,
  kNQURLCharReserved,
  kNQURLCharOther,
};

NQ_EXPORT enum NQURLCharType NQGetURLCharType(char ch);

static inline bool NQIsURLCharUnreserved(char ch)
{
  return NQGetURLCharType(ch) == kNQURLCharUnreserved;
}

static inline bool NQIsURLCharReserved(char ch)
{
  return NQGetURLCharType(ch) == kNQURLCharReserved;
}

static inline bool NQIsURLCharOther(char ch)
{
  return NQGetURLCharType(ch) == kNQURLCharOther;
}

NQ_EXPORT int NQURLEncode(const char* input, size_t inlen, char* output, size_t outlen);
NQ_EXPORT int NQURLDecode(const char* input, size_t inlen, char* output, size_t outlen);
NQ_EXPORT int NQFormURLEncode(const char* input, size_t inlen, char* output, size_t outlen);
NQ_EXPORT int NQFormURLDecode(const char* input, size_t inlen, char* output, size_t outlen);

NQ_EXPORT NQURL* NQURL_create(const char* characters);
NQ_EXPORT void NQURL_destroy(NQURL*);

NQ_EXPORT const char* NQURL_characters(const NQURL*);
NQ_EXPORT size_t NQURL_lenght(NQURL*);
NQ_EXPORT bool NQURL_isEmpty(const NQURL*);

NQ_EXPORT size_t NQURL_getScheme(const NQURL*, char* buffer, size_t length);
NQ_EXPORT size_t NQURL_getHost(const NQURL*, char* buffer, size_t length);
NQ_EXPORT size_t NQURL_getPort(const NQURL*, char* buffer, size_t length);
NQ_EXPORT size_t NQURL_getPath(const NQURL*, char* buffer, size_t length);
NQ_EXPORT size_t NQURL_getQuery(const NQURL*, char* buffer, size_t length);
NQ_EXPORT size_t NQURL_getFragment(const NQURL*, char* buffer, size_t length);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_URL_H */
