/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_APPLICATIONCALLBACKS_H
#define _LIBNETQ_APPLICATIONCALLBACKS_H

typedef struct NQApplication NQApplication;

typedef struct NQApplicationCallbacks {
  void(*onCreate)(NQApplication* app);
  void(*onDestroy)(NQApplication* app);
} NQApplicationCallbacks;

#endif /* _LIBNETQ_APPLICATIONCALLBACKS_H */
