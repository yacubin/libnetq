/*
 * MIT License
 *
 * Copyright (c) 2023-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/GetComputerName.h"

#ifdef NQ_OS_KERNEL
#include <linux/utsname.h>
#include <libnetq/Math.h>
#define NQ_COMPUTER_NAME_MAX (__NEW_UTS_LEN + 1)
#endif

#ifdef NQ_OS_WINDOWS
#include <windows.h>
#include <libnetq/UTF.h>
#define NQ_COMPUTER_NAME_MAX (MAX_COMPUTERNAME_LENGTH + 1)
#endif

#ifdef NQ_OS_UNIX
#include <unistd.h>
#include <string.h>
#define NQ_COMPUTER_NAME_MAX (HOST_NAME_MAX + 1)
#endif

size_t NQGetComputerName(char* buffer, size_t n)
{
#ifdef NQ_OS_KERNEL
  struct new_utsname* u = init_utsname();
  size_t res = strlen(u->nodename);
  memcpy(buffer, u->nodename, NQGetMin(res, n));

  if (res < n)
    buffer[res] = '\0';
  return res;
#endif

#ifdef NQ_OS_WINDOWS
  DWORD size;
  WCHAR wbuf[NQ_COMPUTER_NAME_MAX];
  NQUnicodeInfo info;

  if (GetComputerNameW(wbuf, &size) == TRUE) {
    if (NQConvertUTF16ToUTF8(wbuf, wbuf + sizeof(wbuf), (uint8_t*)buffer, (uint8_t*)buffer + n, &info)) {
      return info.utf8Size;
    }
  }
#endif

#ifdef NQ_OS_UNIX
  if (gethostname(buffer, n) == 0) {
    return strlen(buffer);
  }
#endif

  if (n != 0)
    *buffer = '\0';

  return 0;
}
