/*
 * MIT License
 *
 * Copyright (c) 2025-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/dbc/DBCAlloc.h"

#include <libnetq/Malloc.h>

void* NQDBCAllocDefault(void* allocator, size_t size)
{
  (void)allocator;
  return NQMalloc(size);
}

void NQDBCFreeDefault(void* allocator, void* ptr)
{
  (void)allocator;
  NQFree(ptr);
}

void NQDBCFreeNope(void* allocator, void* ptr)
{
  (void)allocator;
  (void)ptr;
}
