/*
 * MIT License
 *
 * Copyright (c) 2021-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_RECT_H
#define _LIBNETQ_RECT_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQRect {
  int32_t left;
  int32_t top;
  int32_t right;
  int32_t bottom;
} NQRect;

#define NQMakeRect(left, top, right, bottom) { left, top, right, bottom }

NQ_EXPORT void NQRect_init(NQRect*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_RECT_H */
