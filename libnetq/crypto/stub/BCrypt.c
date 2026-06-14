/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/crypto/BCrypt.h"

#ifdef NQCONFIG_USE_STUB_BCRYPT

bool NQBCryptHashPassword(const char* password, const void* salt, void* hash)
{
  NQ_UNUSED_PARAM(password);
  NQ_UNUSED_PARAM(salt);
  NQ_UNUSED_PARAM(hash);
  return false;
}

bool NQBCryptVerifyPassword(const char* password, const void* salt, const void* hash)
{
  NQ_UNUSED_PARAM(password);
  NQ_UNUSED_PARAM(salt);
  NQ_UNUSED_PARAM(hash);
  return false;
}

#endif /* NQCONFIG_USE_STUB_BCRYPT */
