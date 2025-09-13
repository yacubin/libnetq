/*
 * MIT License
 *
 * Copyright (c) 2021-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_WINDOWMANAGER_H
#define _LIBNETQ_WINDOWMANAGER_H

#include <libnetq/Window.h>

#ifdef __cplusplus
extern "C" {
#endif
  
typedef struct NQWindow NQWindow;
typedef struct NQWindowManager NQWindowManager;

// NQWindowClassOperations
struct NQWindowManagerOperations {
  size_t sizeInBytes;

  bool (*init) (NQWindowManager*, void* options);
  bool (*finalize) (NQWindowManager*);
  
  NQWindowHandle (*createWindow) (NQWindowManager*, void* userdata, NQWindowCallback callback);
  bool (*destroyWindow) (NQWindowManager*, NQWindowHandle handle);

  bool (*ioctl) (NQWindowManager*, NQWindowHandle handle, int request, void* ptr);

  bool (*resizeTo) (NQWindowManager*, NQWindowHandle handle, int width, int height);
  int (*getTitle) (NQWindowManager*, NQWindowHandle handle, char* s, size_t n);
  bool (*setTitle) (NQWindowManager*, NQWindowHandle handle, const char* s);
  bool (*setVisible) (NQWindowManager*, NQWindowHandle handle, bool value);
  bool (*moveTo) (NQWindowManager*, NQWindowHandle handle, int x, int y);
  bool (*clear) (NQWindowManager*, NQWindowHandle handle);
  bool (*setToolTip) (NQWindowManager*, NQWindowHandle handle, const char* s);
  bool (*setBorderWidth)(NQWindowManager*, NQWindowHandle handle, int width);
};

NQ_EXPORT void NQWindowManagerInitialize(const struct NQWindowManagerOperations* ops, void* argument);
NQ_EXPORT void NQWindowManagerShutdown(void);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_WINDOWMANAGER_H */
