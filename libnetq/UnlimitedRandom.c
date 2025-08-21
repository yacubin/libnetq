/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/UnlimitedRandom.h"

#include <libnetq/Assert.h>

#if defined(NQ_OS_WIN)
# include <windows.h>
# include <wincrypt.h>
#elif defined(NQ_OS_DARWIN)
# include <CommonCrypto/CommonCryptoError.h>
# include <CommonCrypto/CommonRandom.h>
#elif defined(NQ_OS_UNIX)
# include <errno.h>
# include <fcntl.h>
# include <unistd.h>
#else
#error "This configuration doesn't have a strong source of randomness."
#endif

void NQGetUnlimitedRandom(uint8_t* buffer, size_t size)
{
#if defined(NQ_OS_WIN)
  HCRYPTPROV hCryptProv = 0;
  if (!CryptAcquireContextW(&hCryptProv, 0, MS_DEF_PROV_W, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
    NQ_ASSERT_NOT_REACHED();
  if (!CryptGenRandom(hCryptProv, (DWORD)size, buffer))
    NQ_ASSERT_NOT_REACHED();
  CryptReleaseContext(hCryptProv, 0);

#elif defined(NQ_OS_DARWIN)
  NQ_ALWAYS_ASSERT(CCRandomGenerateBytes(buffer, size) != kCCSuccess);

#elif defined(NQ_OS_UNIX)
  int fd = open("/dev/urandom", O_RDONLY, 0);
  if (fd < 0)
    NQ_ASSERT_NOT_REACHED();
  ssize_t amountRead = 0;
  while ((size_t)(amountRead) < size) {
    ssize_t currentRead = read(fd, buffer + amountRead, size - (size_t)amountRead);
    if (currentRead != -1)
      amountRead += currentRead;
    else {
      if (!(errno == EAGAIN || errno == EINTR))
        NQ_ASSERT_NOT_REACHED();
    }
  }
  close(fd);

#endif
}
