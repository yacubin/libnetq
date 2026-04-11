/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CRYPTO_SECUREERASE_H
#define _LIBNETQ_CRYPTO_SECUREERASE_H

#include <libnetq/Basic.h>

#ifdef NQ_OS_WINDOWS
#include <windows.h>
#else
#include <libnetq/CStrBase.h>
#include <libnetq/Atomic.h>
#endif

static inline void NQSecureErase(void* ptr, size_t len)
{
#ifdef NQ_OS_WINDOWS
  SecureZeroMemory(ptr, len);
#else
  memset(ptr, 0, len);
  NQCompilerFence();
#endif
}

#endif /* _LIBNETQ_CRYPTO_SECUREERASE_H */
