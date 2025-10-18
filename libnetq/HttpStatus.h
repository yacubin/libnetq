/*
 * MIT License
 *
 * Copyright (c) 2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_HTTPSTATUS_H
#define _LIBNETQ_HTTPSTATUS_H

/* 1xx – Informational Responses */
#define NQ_HTTP_STATUS_CONTINUE               100
#define NQ_HTTP_STATUS_SWITCHING_PROTOCOLS    101
#define NQ_HTTP_STATUS_PROCESSING             102
#define NQ_HTTP_STATUS_EARLY_HINTS            103

/* 2xx – Success */
#define NQ_HTTP_STATUS_OK                     200
#define NQ_HTTP_STATUS_CREATED                201

/* 3xx – Redirection */
#define NQ_HTTP_STATUS_MOVED_PERMANENTLY      301
#define NQ_HTTP_STATUS_TEMPORARY_REDIRECT     307

/* 4xx – Client Errors */
#define NQ_HTTP_STATUS_NOT_FOUND              404
#define NQ_HTTP_STATUS_PRECONDITION_FAILED    412
#define NQ_HTTP_STATUS_UPGRADE_REQUIRED       426

/* 5xx – Server Errors */
#define NQ_HTTP_STATUS_INTERNAL_SERVER_ERROR  500

static inline bool NQIsHttpStatusInformational(int code)
{
  return 100 <= code && code < 200;
}

static inline bool NQIsHttpStatusSuccess(int code)
{
  return 200 <= code && code < 300;
}

static inline bool NQIsHttpStatusRedirection(int code)
{
  return 300 <= code && code < 400;
}

static inline bool NQIsHttpStatusClientError(int code)
{
  return 400 <= code && code < 500;
}

static inline bool NQIsHttpStatusServerError(int code)
{
  return 500 <= code && code < 600;
}

#endif /* _LIBNETQ_HTTPSTATUS_H */
