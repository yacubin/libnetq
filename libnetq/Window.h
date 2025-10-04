/*
 * MIT License
 *
 * Copyright (c) 2021-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_WINDOW_H
#define _LIBNETQ_WINDOW_H

#include <libnetq/Basic.h>
#include <libnetq/Event.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NQWindowInvalid (0)

typedef uintptr_t NQWindowHandle;
typedef bool (*NQWindowCallback) (void*, NQWindowHandle, NQEvent*);

NQ_EXPORT NQWindowHandle NQWindowCreate(void* userdata, NQWindowCallback callback);
NQ_EXPORT bool NQWindowDestroy(NQWindowHandle handle);

enum {
  NQ_WINDOW_IOCTL_HANDLE = 0x3112,
  NQ_WINDOW_IOCTL_DISPLAY = 0x3113,
  NQ_WINDOW_IOCTL_GC = 0x3114,
  NQ_WINDOW_IOCTL_PIXMAP = 0x3115,
  NQ_WINDOW_IOCTL_IMAGE = 0x3116,
  NQ_WINDOW_IOCTL_ANATIVE_CREATED = 0x3117,
  NQ_WINDOW_IOCTL_ANATIVE_RESIZED = 0x3118,
  NQ_WINDOW_IOCTL_ANATIVE_REDRAW_NEEDED = 0x3119,
  NQ_WINDOW_IOCTL_ANATIVE_DESTROYED = 0x311A,
  NQ_WINDOW_IOCTL_ANATIVE_CONTENT_RECT_CHANGED = 0x311B,
};

NQ_EXPORT bool NQWindowIoctl(NQWindowHandle handle, int request, void*);

NQ_EXPORT bool NQWindowResizeTo(NQWindowHandle handle, int width, int height);
NQ_EXPORT int NQWindowGetTitle(NQWindowHandle handle, char* s, size_t n);
NQ_EXPORT bool NQWindowSetTitle(NQWindowHandle handle, const char* s);
NQ_EXPORT bool NQWindowSetVisible(NQWindowHandle handle, bool value);
NQ_EXPORT bool NQWindowMoveTo(NQWindowHandle handle, int x, int y);
NQ_EXPORT bool NQWindowClear(NQWindowHandle handle);
NQ_EXPORT bool NQWindowSetToolTip(NQWindowHandle handle, const char* s);
NQ_EXPORT bool NQWindowSetBorderWidth(NQWindowHandle handle, int width);

NQ_EXPORT const char* NQWindowGetClassName(void);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_WINDOW_H */
