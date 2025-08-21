/*
 * MIT License
 *
 * Copyright (c) 2022-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_ALLOCATOR_H
#define _LIBNETQ_ALLOCATOR_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQAllocator NQAllocator;

struct NQAllocatorCallbacks {
  void* (*alloc) (NQAllocator*, size_t size);
  void (*free) (NQAllocator*, void* ptr);
};

struct NQAllocator {
  struct NQAllocatorCallbacks* callbacks;
};

inline void* NQAllocator_alloc(NQAllocator* thiz, size_t size)
{
  return thiz->callbacks->alloc(thiz, size);
}

inline void* NQAllocator_free(NQAllocator* thiz, void* ptr)
{
  return thiz->callbacks->free(thiz, ptr);
}

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _LIBNETQ_ALLOCATOR_H */
