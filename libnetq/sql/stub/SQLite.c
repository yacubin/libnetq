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

#ifdef NQCONFIG_USE_STUB_SQLITE

const char* NQSQLiteDatabase_version(void)
{
  return NULL;
}

NQSQLiteDatabase* NQSQLiteDatabase_open(const char* filename, int mode)
{
  NQ_UNUSED_PARAM(filename);
  NQ_UNUSED_PARAM(mode);
  return NULL;
}

void NQSQLiteDatabase_release(NQSQLiteDatabase* thiz)
{
  NQ_UNUSED_PARAM(thiz);
}

bool NQSQLiteDatabase_exec(NQSQLiteDatabase* thiz, const char* query)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(query);
  return false;
}

size_t NQSQLiteStatement_changes(NQSQLiteDatabase* thiz)
{
  NQ_UNUSED_PARAM(thiz);
  return 0;
}

NQSQLiteStatement* NQSQLiteDatabase_prepare(NQSQLiteDatabase* thiz, const char* query)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(query);
  return NULL;
}

void NQSQLiteStatement_release(NQSQLiteStatement* thiz)
{
  NQ_UNUSED_PARAM(thiz);
}

bool NQSQLiteStatement_step(NQSQLiteStatement* thiz, bool* done)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(done);
  return false;
}

bool NQSQLiteStatement_bindNull(NQSQLiteStatement* thiz, size_t index)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(index);
  return false;
}

bool NQSQLiteStatement_bindInt32(NQSQLiteStatement* thiz, size_t index, int32_t number)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(index);
  NQ_UNUSED_PARAM(number);
  return false;
}

bool NQSQLiteStatement_bindInt64(NQSQLiteStatement* thiz, size_t index, int64_t number)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(index);
  NQ_UNUSED_PARAM(number);
  return false;
}

#ifdef NQ_HAS_COMPILER_SSE
bool NQSQLiteStatement_bindDouble(NQSQLiteStatement* thiz, size_t index, double number)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(index);
  NQ_UNUSED_PARAM(number);
  return false;
}
#endif

bool NQSQLiteStatement_bindText(NQSQLiteStatement* thiz, size_t index, const char* text)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(index);
  NQ_UNUSED_PARAM(text);
  return false;
}

bool NQSQLiteStatement_bindText2(NQSQLiteStatement* thiz, size_t index, const char* text, size_t size)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(index);
  NQ_UNUSED_PARAM(text);
  NQ_UNUSED_PARAM(size);
  return false;
}

bool NQSQLiteStatement_bindBlob(NQSQLiteStatement* thiz, size_t index, const void* data, size_t size)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(index);
  NQ_UNUSED_PARAM(data);
  NQ_UNUSED_PARAM(size);
  return false;
}

int NQSQLiteStatement_columnType(NQSQLiteStatement* thiz, size_t index)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(index);
  return kNQSQLiteValueNull;
}

size_t NQSQLiteStatement_columnSize(NQSQLiteStatement* thiz, size_t index)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(index);
  return 0;
}

int32_t NQSQLiteStatement_columnInt32(NQSQLiteStatement* thiz, size_t index)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(index);
  return 0;
}

int64_t NQSQLiteStatement_columnInt64(NQSQLiteStatement* thiz, size_t index)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(index);
  return 0;
}

#ifdef NQ_HAS_COMPILER_SSE
double NQSQLiteStatement_columnDouble(NQSQLiteStatement* thiz, size_t index)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(index);
  return 0;
}
#endif

const char* NQSQLiteStatement_columnText(NQSQLiteStatement* thiz, size_t index)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(index);
  return NULL;
}

const void* NQSQLiteStatement_columnBlob(NQSQLiteStatement* thiz, size_t index)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(index);
  return NULL;
}

#endif
