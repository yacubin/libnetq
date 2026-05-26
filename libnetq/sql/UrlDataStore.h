/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_SQL_URLDATASTORE_H
#define _LIBNETQ_SQL_URLDATASTORE_H

#include <libnetq/sql/SQLite.h>

#ifdef __cplusplus
extern "C" {
#endif

NQ_EXPORT bool NQUrlDataStoreInit(NQSQLiteDatabase* database);
NQ_EXPORT void NQUrlDataStoreExit(NQSQLiteDatabase* database);
NQ_EXPORT bool NQUrlDataStoreMkdir(NQSQLiteDatabase* database, int32_t userId, const char* path);
NQ_EXPORT int NQUrlDataStoreRead(NQSQLiteDatabase* database, int32_t userId, const char* path, char* buf, size_t len);
NQ_EXPORT int NQUrlDataStoreWrite(NQSQLiteDatabase* database, int32_t userId, const char* path, const char* buf, size_t len);

typedef bool (*NQUrlDataStoreCallback) (void* userdata, const char* name, const char* val, size_t len);
NQ_EXPORT bool NQUrlDataStoreList(NQSQLiteDatabase* database, int32_t userId, const char* path, NQUrlDataStoreCallback callback, void* userdata);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_SQL_URLDATASTORE_H */
