/*
 * MIT License
 *
 * Copyright (c) 2023-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/GetUserName.h"

#ifdef NQ_OS_WINDOWS
#include <windows.h>
#include <libnetq/UTF.h>
#endif

#ifdef NQ_OS_UNIX
#include <unistd.h>
#include <limits.h>
#include <string.h>
#endif

size_t NQGetUserName(char* buffer, size_t n)
{
  if (n > 0) {

#ifdef NQ_OS_WINDOWS
    WCHAR wbuf[128];
    DWORD size = NQ_ARRAY_LENGTH(wbuf);
    NQUnicodeInfo info;

    if (GetUserNameW(wbuf, &size) && size < NQ_ARRAY_LENGTH(wbuf)) {
      if (NQConvertUTF16ToUTF8(wbuf, wbuf + size, (uint8_t*)buffer, (uint8_t*)buffer + n, &info)) {
        return info.utf8Size;
      }
    }
#endif

#ifdef NQ_OS_UNIX

#ifdef HAVE_GETLOGIN_R
    int ret = getlogin_r(buffer, n);
    if (ret != 0) {
      return (size_t)ret;
    }
#endif

    const char* str = getlogin();
    if (NULL != str) {
      size_t len = strlen(str);
      if (len < n)
        memcpy(buffer, str, len + 1);
      else {
        memcpy(buffer, str, n - 1);
        buffer[n - 1] = '\0';
      }
      return len;
    }
#endif

    *buffer = '\0';
  }

  return 0;
}
