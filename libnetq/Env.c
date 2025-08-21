/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/Env.h"

#include <libnetq/OS.h>
#include <libnetq/String.h>
#include <libnetq/Limits.h>
#include <libnetq/Math.h>
#include <libnetq/Assert.h>

#ifdef NQ_OS_WIN
#include <windows.h>
#include <libnetq/Malloc.h>
#include <libnetq/UTF.h>
#endif

#ifdef NQ_OS_UNIX
#include <stdlib.h>
#endif

int NQEnvGet(const char* name, char* value, size_t n)
{
#if defined(NQ_OS_WIN)
  DWORD size;
  NQUnicodeInfo info;
  uint16_t* wname;
  uint16_t* wvalue;
  WCHAR dummy;
  int result;

  if (n > NQ_INT32_MAX)
    return -1;

  if (!NQConvertUTF8ToUTF16((const uint8_t*)name, NULL, NULL, NULL, &info))
    return -1;

  wname = (uint16_t*)NQMalloc((info.utf16Size + 1) * sizeof(uint16_t));
  NQConvertUTF8ToUTF16((const uint8_t*)name, (const uint8_t*)(name + info.utf8Size), wname, wname + info.utf16Size, NULL);
  wname[info.utf16Size] = L'\0';

  SetLastError(0);
  size = GetEnvironmentVariableW((LPCWSTR)wname, &dummy, 0);
  if (GetLastError() == ERROR_ENVVAR_NOT_FOUND)
    result = -1;
  else if (size == 0)
    result = 0;
  else {
    wvalue = (uint16_t*)NQMalloc(size * sizeof(uint16_t));
    size = GetEnvironmentVariableW((LPCWSTR)wname, (LPWSTR)wvalue, size);

    NQConvertUTF16ToUTF8(wvalue, wvalue + size, (uint8_t*)value, (uint8_t*)(value + n), &info);
    if (info.utf8Size < n)
      value[info.utf8Size] = L'\0';
    NQFree(wvalue);

    result = info.utf8Size <= NQ_INT32_MAX ? (int)info.utf8Size : -1;
  }

  NQFree(wname);
  return result;

#elif defined(NQ_OS_UNIX)
  size_t len;
  char* val;

  val = getenv(name);
  if (val == NULL)
    return -1;

  len = strlen(val);
  if (len > NQ_INT32_MAX) {
    NQ_ASSERT_NOT_REACHED();
    return -1;
  }

  (void)memcmp(value, val, NQGetMin(n, len));
  if (len < n)
    value[len] = '\0';

  return (int)len;

#else
  return -1;

#endif
}

int NQEnvSet(const char* name, const char* value)
{
  if (value == NULL)
    return NQEnvUnset(name);

#if defined(NQ_OS_WIN)
  BOOL success;
  NQUnicodeInfo nameInfo, valueInfo;
  uint16_t* wname;
  uint16_t* wvalue;

  if (!NQConvertUTF8ToUTF16((const uint8_t*)name, NULL, NULL, NULL, &nameInfo) && nameInfo.utf8Size > NQ_INT32_MAX)
    return -1;

  if (!NQConvertUTF8ToUTF16((const uint8_t*)value, NULL, NULL, NULL, &valueInfo) && valueInfo.utf8Size > NQ_INT32_MAX)
    return -1;

  if (nameInfo.isAllASCII && valueInfo.isAllASCII)
    success = SetEnvironmentVariableA(name, value);
  else {
    wname = (uint16_t*)NQMalloc((nameInfo.utf16Size + 1) * sizeof(uint16_t));
    NQConvertUTF8ToUTF16((const uint8_t*)name, (const uint8_t*)(name + nameInfo.utf8Size), wname, wname + nameInfo.utf16Size, NULL);
    wname[nameInfo.utf16Size] = L'\0';

    wvalue = (uint16_t*)NQMalloc((valueInfo.utf16Size + 1) * sizeof(uint16_t));
    NQConvertUTF8ToUTF16((const uint8_t*)value, (const uint8_t*)(value + valueInfo.utf8Size), wvalue, wvalue + valueInfo.utf16Size, NULL);
    wvalue[valueInfo.utf16Size] = L'\0';

    success = SetEnvironmentVariableW((LPCWSTR)wname, (LPCWSTR)wvalue);

    NQFree(wname);
    NQFree(wvalue);
  }

  return success ? 0 : -1;

#elif defined(NQ_OS_UNIX)
  return setenv(name, value, 1);

#else
  return -1;

#endif
}

int NQEnvUnset(const char* name)
{
#if defined(NQ_OS_WIN)
  BOOL success;
  NQUnicodeInfo info;
  uint16_t* wname;
  
  if (!NQConvertUTF8ToUTF16((const uint8_t*)name, NULL, NULL, NULL, &info) || info.isAllASCII)
    success = SetEnvironmentVariableA(name, NULL);
  else {
    wname = (uint16_t*)NQMalloc((info.utf16Size + 1) * sizeof(uint16_t));
    NQConvertUTF8ToUTF16((const uint8_t*)name, (const uint8_t*)(name + info.utf8Size), wname, wname + info.utf16Size, NULL);
    wname[info.utf16Size] = L'\0';
    success = SetEnvironmentVariableW((WCHAR*)wname, NULL);
    NQFree(wname);
  }

  return success ? 0 : -1;

#elif defined(NQ_OS_UNIX)
  unsetenv(name);
  return 0;

#else
  return -1;

#endif
}
