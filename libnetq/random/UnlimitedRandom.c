/*
 * MIT License
 *
 * Copyright (c) 2020-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/random/UnlimitedRandom.h"

#include <libnetq/Assert.h>
#include <libnetq/ErrorCode.h>

#if defined(NQ_SYS_LINUX)
# include <linux/random.h>
#elif defined(NQ_OS_WINDOWS)
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
# error "This configuration doesn't have a strong source of randomness."
#endif

int NQGetUnlimitedRandom(void* data, size_t size)
{
#if defined(NQ_SYS_LINUX)
  get_random_bytes(data, size);
  return 0;

#elif defined(NQ_OS_WINDOWS)
  HCRYPTPROV hCryptProv = 0;
  if (!CryptAcquireContextW(&hCryptProv, 0, MS_DEF_PROV_W, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
    return -(int)GetLastError();

  int res = 0;
  if (!CryptGenRandom(hCryptProv, (DWORD)size, (BYTE*)data))
    res = -(int)GetLastError();
  CryptReleaseContext(hCryptProv, 0);
  return res;

#elif defined(NQ_OS_DARWIN)
  if (CCRandomGenerateBytes(data, size) != kCCSuccess)
    return -NQ_ENOTSUP;
  return 0;

#elif defined(NQ_OS_UNIX)
  int fd = open("/dev/urandom", O_RDONLY, 0);
  if (fd < 0)
    NQ_ASSERT_NOT_REACHED();
  ssize_t amountRead = 0;
  while ((size_t)(amountRead) < size) {
    ssize_t currentRead = read(fd, (char*)data + amountRead, size - (size_t)amountRead);
    if (currentRead != -1)
      amountRead += currentRead;
    else {
      if (!(errno == EAGAIN || errno == EINTR))
        NQ_ASSERT_NOT_REACHED();
    }
  }
  close(fd);
  return 0;

#endif
}
