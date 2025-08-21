/*
 * MIT License
 *
 * Copyright (c) 2022-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/CStrBase.h"

#include <string.h>
#include <libnetq/Malloc.h>

const char* NQCStrEmpty()
{
  static const char s_cstrEmpty[] = "";
  return s_cstrEmpty;
}

size_t NQCStrLen(const uint8_t* s)
{
  const uint8_t* p = s;
  while (*p)
    p++;
  return p - s;
}

size_t NQCStrLen16(const uint16_t* s)
{
  const uint16_t* p = s;
  while (*p)
    p++;
  return p - s;
}

char* NQCStrDuplicate(const char* str)
{
  size_t len = strlen(str) + 1;

  char* dup = (char*)NQMalloc(len);
  if (dup == NULL)
    return NULL;

  memcpy(dup, str, len);
  return dup;
}

char* NQCStrDuplicateWithLength(const char* str, size_t len)
{
  char* dup = (char*)NQMalloc(len + 1);
  if (dup == NULL)
    return NULL;

  memcpy(dup, str, len);
  dup[len] = '\0';
  return dup;
}

const char* NQCStrFindCStrn(const char* s1, const char* s2, size_t n)
{
  size_t sz = strlen(s1);
  while (sz < n) {
    if (!memcmp(s1, s2, sz))
      return s2;
    s2++;
    n--;
  }
  return NULL;
}

void NQCStrDestroy(const char* str)
{
  NQFree((char*)str);
}

bool NQCStrStartsWith(const char* str, const char* search)
{
  while (*search) {
    if (*str == '\0' || *str != *search)
      return false;
    str++;
    search++;
  }

  return true;
}
