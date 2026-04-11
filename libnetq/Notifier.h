/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_NOTIFIER_H
#define _LIBNETQ_NOTIFIER_H

typedef struct NQWebNotifierEntry NQWebNotifierEntry;
typedef int (*NQWebNotifierFunction) (struct NQWebNotifierEntry* notifier, unsigned event, void* data);

struct NQWebNotifierEntry {
  NQWebNotifierFunction callback;
  struct NQWebNotifierEntry* next;
};

#endif /* _LIBNETQ_NOTIFIER_H */
