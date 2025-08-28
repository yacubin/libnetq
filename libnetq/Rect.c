/*
 * MIT License
 *
 * Copyright (c) 2021-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/Rect.h"

void NQRect_init(NQRect* thiz)
{
  thiz->left = 0;
  thiz->top = 0;
  thiz->right = 0;
  thiz->bottom = 0;
}
