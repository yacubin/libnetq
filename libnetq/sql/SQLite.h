/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_SQL_SQLITE_H
#define _LIBNETQ_SQL_SQLITE_H

#include <libnetq/Basic.h>

#ifdef NQCONFIG_USE_SQLITE3_SQLITE
#include <sqlite3.h>
typedef sqlite3 NQSQLiteDatabase;
typedef sqlite3_stmt NQSQLiteStatement;
enum NQSQLiteValueType {
  kNQSQLiteValueNull    = SQLITE_NULL,
  kNQSQLiteValueInteget = SQLITE_INTEGER,
  kNQSQLiteValueFloat   = SQLITE_FLOAT,
  kNQSQLiteValueText    = SQLITE_TEXT,
  kNQSQLiteValueBlob    = SQLITE_BLOB,
};
#endif

#ifdef NQCONFIG_USE_STUB_SQLITE
typedef void NQSQLiteDatabase;
typedef void NQSQLiteStatement;
enum NQSQLiteValueType {
  kNQSQLiteValueInteget,
  kNQSQLiteValueFloat,
  kNQSQLiteValueBlob,
  kNQSQLiteValueNull,
  kNQSQLiteValueText,
};
#endif

#ifdef __cplusplus
extern "C" {
#endif

enum NQSQLiteOpenMode {
  kNQSQLiteOpenReadOnly,
  kNQSQLiteOpenReadWrite,
  kNQSQLiteOpenCreateReadWrite,
};

NQ_EXPORT const char* NQSQLiteDatabase_version(void);

NQ_EXPORT NQSQLiteDatabase* NQSQLiteDatabase_open(const char* filename, int mode);
NQ_EXPORT void NQSQLiteDatabase_release(NQSQLiteDatabase*);

NQ_EXPORT bool NQSQLiteDatabase_exec(NQSQLiteDatabase*, const char* query);
NQ_EXPORT size_t NQSQLiteStatement_changes(NQSQLiteDatabase*);

NQ_EXPORT NQSQLiteStatement* NQSQLiteDatabase_prepare(NQSQLiteDatabase*, const char* query);
NQ_EXPORT void NQSQLiteStatement_release(NQSQLiteStatement*);

NQ_EXPORT bool NQSQLiteStatement_step(NQSQLiteStatement*, bool* done);

NQ_EXPORT bool NQSQLiteStatement_bindNull(NQSQLiteStatement*, size_t index);
NQ_EXPORT bool NQSQLiteStatement_bindInt32(NQSQLiteStatement*, size_t index, int32_t number);
NQ_EXPORT bool NQSQLiteStatement_bindInt64(NQSQLiteStatement*, size_t index, int64_t number);
NQ_EXPORT bool NQSQLiteStatement_bindDouble(NQSQLiteStatement*, size_t index, double number);
NQ_EXPORT bool NQSQLiteStatement_bindText(NQSQLiteStatement*, size_t index, const char* text);
NQ_EXPORT bool NQSQLiteStatement_bindText2(NQSQLiteStatement*, size_t index, const char* text, size_t size);
NQ_EXPORT bool NQSQLiteStatement_bindBlob(NQSQLiteStatement*, size_t index, const void* data, size_t size);

NQ_EXPORT int NQSQLiteStatement_columnType(NQSQLiteStatement*, size_t index);
NQ_EXPORT size_t NQSQLiteStatement_columnSize(NQSQLiteStatement*, size_t index);

NQ_EXPORT int32_t NQSQLiteStatement_columnInt32(NQSQLiteStatement*, size_t index);
NQ_EXPORT int64_t NQSQLiteStatement_columnInt64(NQSQLiteStatement*, size_t index);
NQ_EXPORT double NQSQLiteStatement_columnDouble(NQSQLiteStatement*, size_t index);
NQ_EXPORT const char* NQSQLiteStatement_columnText(NQSQLiteStatement*, size_t index);
NQ_EXPORT const void* NQSQLiteStatement_columnBlob(NQSQLiteStatement*, size_t index);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_SQL_SQLITE_H */
