/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/sql/SQLite.h"

#ifdef NQCONFIG_USE_SQLITE3_SQLITE

#include <libnetq/Limits.h>
#include <libnetq/Assert.h>
#include <libnetq/Log.h>

static inline int TO_INT(size_t n)
{
  NQ_ASSERT(n <= NQ_INT32_MAX);
  return (int)n;
}

const char* NQSQLiteDatabase_version(void)
{
  return sqlite3_libversion();
}

NQSQLiteDatabase* NQSQLiteDatabase_open(const char* filename, int mode)
{
  int flags = SQLITE_OPEN_AUTOPROXY;
  switch (mode) {
  case kNQSQLiteOpenReadOnly:
    flags |= SQLITE_OPEN_READONLY;
    break;
  case kNQSQLiteOpenReadWrite:
    flags |= SQLITE_OPEN_READWRITE;
    break;
  case kNQSQLiteOpenCreateReadWrite:
    flags |= SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE;
    break;
  }

  NQSQLiteDatabase* thiz;
  int ret = sqlite3_open_v2(filename, &thiz, flags, NULL);
  if (ret != SQLITE_OK) {
    NQ_LOGE("sqlite3_open_v2 failed %i, %s", ret, sqlite3_errmsg(thiz));
    return NULL;
  }

  return thiz;
}

void NQSQLiteDatabase_release(NQSQLiteDatabase* thiz)
{
  int ret = sqlite3_close(thiz);
  if (SQLITE_OK != ret) {
    NQ_LOGE("sqlite3_close failed %i, %s", ret, sqlite3_errmsg(thiz));
  }
}

bool NQSQLiteDatabase_exec(NQSQLiteDatabase* thiz, const char* query)
{
  NQSQLiteStatement* statement = NQSQLiteDatabase_prepare(thiz, query);
  if (!statement)
    return false;

  bool done;
  bool sucess = NQSQLiteStatement_step(statement, &done);
  NQSQLiteStatement_release(statement);
  return sucess && done;
}

size_t NQSQLiteStatement_changes(NQSQLiteDatabase* thiz)
{
  return (size_t)sqlite3_changes(thiz);
}

NQSQLiteStatement* NQSQLiteDatabase_prepare(NQSQLiteDatabase* thiz, const char* query)
{
  int ret;

  const char* tail = NULL;
  sqlite3_stmt* stmt = NULL;

  ret = sqlite3_prepare_v2(thiz, query, -1, &stmt, &tail);
  if (SQLITE_OK != ret) {
    NQ_LOGE("sqlite3_prepare_v2 failed %i, %s", ret, sqlite3_errmsg(thiz));
    return NULL;
  }

  if (tail && *tail != '\0') {
    NQ_LOGE("sqlite3_prepare_v2 with tail - %s", tail);
    sqlite3_finalize(stmt);
    return NULL;
  }

  return stmt;
}

void NQSQLiteStatement_release(NQSQLiteStatement* thiz)
{
  NQ_ASSERT(thiz != NULL);
  sqlite3_finalize(thiz);
}

bool NQSQLiteStatement_step(NQSQLiteStatement* thiz, bool* done)
{
  NQ_ASSERT(thiz != NULL);

  int ret = sqlite3_step(thiz);
  switch (ret) {
  case SQLITE_ROW:
    if (done)
      *done = false;
    return true;

  case SQLITE_DONE:
    if (done)
      *done = true;
    return true;

  default:
    NQ_LOGE("sqlite3_step failed %i", ret /* sqlite3_errmsg(db) */);
    return false;
  };
}

bool NQSQLiteStatement_bindNull(NQSQLiteStatement* thiz, size_t index)
{
  return sqlite3_bind_null(thiz, TO_INT(index)) == SQLITE_OK;
}

bool NQSQLiteStatement_bindInt32(NQSQLiteStatement* thiz, size_t index, int32_t number)
{
  return sqlite3_bind_int(thiz, TO_INT(index), number) == SQLITE_OK;
}

bool NQSQLiteStatement_bindInt64(NQSQLiteStatement* thiz, size_t index, int64_t number)
{
  return sqlite3_bind_int64(thiz, TO_INT(index), number) == SQLITE_OK;
}

bool NQSQLiteStatement_bindDouble(NQSQLiteStatement* thiz, size_t index, double number)
{
  return sqlite3_bind_double(thiz, TO_INT(index), number) == SQLITE_OK;
}

bool NQSQLiteStatement_bindText(NQSQLiteStatement* thiz, size_t index, const char* text)
{
  return sqlite3_bind_text(thiz, TO_INT(index), text, -1, SQLITE_TRANSIENT) == SQLITE_OK;
}

bool NQSQLiteStatement_bindText2(NQSQLiteStatement* thiz, size_t index, const char* text, size_t size)
{
  return sqlite3_bind_text(thiz, TO_INT(index), text, TO_INT(size), SQLITE_TRANSIENT) == SQLITE_OK;
}

bool NQSQLiteStatement_bindBlob(NQSQLiteStatement* thiz, size_t index, const void* data, size_t size)
{
  return sqlite3_bind_blob(thiz, TO_INT(index), data, TO_INT(size), SQLITE_TRANSIENT) == SQLITE_OK;
}

int NQSQLiteStatement_columnType(NQSQLiteStatement* thiz, size_t index)
{
  return sqlite3_column_type(thiz, TO_INT(index));
}

size_t NQSQLiteStatement_columnSize(NQSQLiteStatement* thiz, size_t index)
{
  return sqlite3_column_bytes(thiz, TO_INT(index));
}

int32_t NQSQLiteStatement_columnInt32(NQSQLiteStatement* thiz, size_t index)
{
  return sqlite3_column_int(thiz, TO_INT(index));
}

int64_t NQSQLiteStatement_columnInt64(NQSQLiteStatement* thiz, size_t index)
{
  return sqlite3_column_int64(thiz, TO_INT(index));
}

double NQSQLiteStatement_columnDouble(NQSQLiteStatement* thiz, size_t index)
{
  return sqlite3_column_double(thiz, TO_INT(index));
}

const char* NQSQLiteStatement_columnText(NQSQLiteStatement* thiz, size_t index)
{
  return (const char*)sqlite3_column_text(thiz, TO_INT(index));
}

const void* NQSQLiteStatement_columnBlob(NQSQLiteStatement* thiz, size_t index)
{
  return sqlite3_column_blob(thiz, TO_INT(index));
}

#endif
