/*
 * MIT License
 *
 * Copyright (c) 2025-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_DBC_DBCID_H
#define _LIBNETQ_DBC_DBCID_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t NQDBCID;

#define kNQDBCExtendedIdMask (0x80000000u)
#define kNQDBCIdMask (~kNQDBCExtendedIdMask)

#define NQDBCIsExtendedId(id) ((id) & kNQDBCExtendedIdMask)
#define NQDBCGetId(id) ((id) & kNQDBCIdMask)

#define NQDBC_EXTENDED_ID_BITS (29u)
#define NQDBC_BASE_ID_BITS (11u)

NQ_EXPORT uint32_t NQDBCGetIdBits(NQDBCID id);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _LIBNETQ_DBC_DBCID_H */
