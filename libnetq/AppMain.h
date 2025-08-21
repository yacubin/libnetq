/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_APPMAIN_H
#define _LIBNETQ_APPMAIN_H

#include <libnetq/OS.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*NQMainEntryPoint) (int argc, const char* argv[]);

#ifdef NQ_OS_WIN
#include <windows.h>
#endif

#ifdef NQ_OS_ANDROID
#include <android/native_activity.h>
#endif

extern int NQAppMain(int argc, const char* argv[]);

#ifdef NQ_OS_WIN
extern INT NQAppMainWin32A(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT iCmdShow);
extern INT NQAppMainWin32W(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, INT iCmdShow);
#endif

#ifdef NQ_OS_ANDROID
extern void NQAppNativeActivity(ANativeActivity* activity, void* savedState, size_t savedStateSize);
#endif

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_APPMAIN_H */
