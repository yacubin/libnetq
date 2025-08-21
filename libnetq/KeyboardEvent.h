/*
 * MIT License
 *
 * Copyright (c) 2021-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_KEYBOARDEVENT_H
#define _LIBNETQ_KEYBOARDEVENT_H

#include <libnetq/Event.h>

typedef struct NQKeyboardEvent {
  int type;
  uint32_t scanCode;
  uint32_t repeatCount;
  char key;
} NQKeyboardEvent;

#endif /* _LIBNETQ_KEYBOARDEVENT_H */
