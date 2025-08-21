/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/Malloc.h"

#include <string.h>
#include <libnetq/OS.h>

#ifdef NQ_OS_WINDOWS
#include <windows.h>
#endif

#ifdef NQ_OS_UNIX
#include <stdlib.h>
#endif

void* NQMalloc(size_t n)
{
  return malloc(n);
}

void* NQZeroMalloc(size_t n)
{
  void* result = malloc(n);
  memset(result, 0, n);
  return result;
}

void* NQCalloc(size_t n_elements, size_t element_size)
{
  return calloc(n_elements, element_size);
}

void* NQRealloc(void* p, size_t n)
{
  return realloc(p, n);
}

void NQFree(void* p)
{
  free(p);
}
