/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/Math.h"

#include <libnetq/CPU.h>
#include <libnetq/Compiler.h>

/*
  nq_gcd64 500 loop

  1. 0.5022 (sec)
  2. 0.3256 (sec)
  3. 0.0063 (sec)
  4. 0.0007 (sec)
  5. 0.0008 (sec)
  6. 0.0006 (sec)
  7. 0.0003 (sec)
  8. 0.0002 (sec)
*/
#define NQ_GCD_TYPE 8

#if NQ_GCD_TYPE == 1
int64_t nq_gcd64(int64_t a, int64_t b)
{
  int64_t i;
  int64_t result;

  result = 1;
  for (i = a; i > 0; i--) {
    if (a % i == 0 && b % i == 0) {
      result = i;
      break;
    }
  }

  return result;
}
#elif NQ_GCD_TYPE == 2
int64_t nq_gcd64(int64_t a, int64_t b)
{
  int64_t i;
  int64_t result;

  for (i = nq_min(a, b); i > 0; i--) {
    if (a % i == 0 && b % i == 0) {
      result = i;
      break;
    }
  }

  return result;
}
#elif NQ_GCD_TYPE == 3
int64_t nq_gcd64(int64_t a, int64_t b)
{
  int64_t i;
  int64_t temp;
  int64_t result;

  result = 1;

  if (a > b) {
    temp = a;
    a = b;
    b = temp;
  }

  while (a > 1 && b > 1) {
    for (i = 2; i <= a; i++) {
      if (a % i == 0 && b % i == 0) {
        result *= i;
        a /= i;
        b /= i;
        break;
      }

      if (a % i == 0) {
        a /= i;
        break;
      }
      if (b % i == 0) {
        b /= i;
        break;
      }
    }
  }

  return result;
}
#elif NQ_GCD_TYPE == 4
int64_t nq_gcd64(int64_t a, int64_t b)
{
  int64_t temp;

  if (a == b)
    return a;

  if (a > b) {
    temp = a;
    a = b;
    b = temp;
  }

  return nq_gcd64(a, b - a);
}
#elif NQ_GCD_TYPE == 5
int64_t nq_gcd64(int64_t a, int64_t b)
{
  int64_t temp;

  while (a != b) {
    if (a > b) {
      temp = a;
      a = b;
      b = temp;
    }
    b = b - a;
  }

  return a;
}
#elif NQ_GCD_TYPE == 6
int64_t nq_gcd64(int64_t a, int64_t b)
{
  if (a == 0)
    return b;

  if (b == 0)
    return a;

  if (a == b)
    return a;

  if (a == 1 || b == 1)
    return 1;

  if (a % 2 == 0 && b % 2 == 0)
    return 2 * nq_gcd64(a / 2, b / 2);

  if (a % 2 == 0 && b % 2 != 0)
    return nq_gcd64(a / 2, b);

  if (a % 2 != 0 && b % 2 == 0)
    return nq_gcd64(a, b / 2);

  if (a < b)
    return nq_gcd64((b - a) / 2, a);
  
  return nq_gcd64((a - b) / 2, b);
}
#elif NQ_GCD_TYPE == 7
int64_t nq_gcd64(int64_t a, int64_t b)
{
  int64_t temp;
  int64_t result;
  
  if (a == 0)
    return b;

  if (b == 0)
    return a;

  if (a == b)
    return a;

  if (a == 1 || b == 1)
    return 1;

  result = 1;
  while (a != 0 && b != 0) {
    if (a % 2 == 0 && b % 2 == 0) {
      result *= 2;
      a /= 2;
      b /= 2;
      continue;
    }

    if (a % 2 == 0 && b % 2 != 0) {
      a /= 2;
      continue;
    }

    if (a % 2 != 0 && b % 2 == 0) {
      b /= 2;
      continue;
    }

    if (a > b) {
      temp = a;
      a = b;
      b = temp;
    }

    temp = a;
    a = (b - a) / 2;
    b = temp;
  }

  if (a == 0)
    return result * b;
  
  return result * a;
}
#else // NQ_GCD_TYPE == 7
int64_t nq_gcd64(int64_t a, int64_t b)
{
  int64_t temp;
  int64_t result;

  if (a == 0)
    return b;

  if (b == 0)
    return a;

  if (a == b)
    return a;

  if (a == 1 || b == 1)
    return 1;

  result = 1;
  while (a != 0 && b != 0) {
    if (((a & 1) | (b & 1)) == 0) {
      result <<= 1;
      a >>= 1;
      b >>= 1;
      continue;
    }

    if (((a & 1) == 0) && (b & 1)) {
      a >>= 1;
      continue;
    }

    if ((a & 1) && ((b & 1) == 0)) {
      b >>= 1;
      continue;
    }

    if (a > b) {
      temp = a;
      a = b;
      b = temp;
    }

    temp = a;
    a = (b - a) >> 1;
    b = temp;
  }

  if (a == 0)
    return result * b;
  
  return result * a;
}
#endif

unsigned nq_clz32(uint32_t number)
{
#if defined(NQ_COMPILER_GCC)
  if (number)
    return __builtin_ctz(number);
  return 32;
#elif defined(NQ_COMPILER_MSVC) && !defined(NQ_CPU_X86)
  unsigned long ret = 0;
  if (_BitScanForward(&ret, number))
    return ret;
  return 32;
#else
  unsigned zeroCount = 0;
  for (int i = 31; i >= 0; i--) {
    if (!(number >> i))
      zeroCount++;
    else
      break;
  }
  return zeroCount;
#endif
#if 0
  unsigned zeroCount = 0;
  for (unsigned i = 0; i < 32; i++) {
    if (number & 1)
      break;

    zeroCount++;
    number >>= 1;
  }
  return zeroCount;
#endif
}

unsigned nq_clz64(uint64_t number)
{
#if defined(NQ_COMPILER_GCC)
  if (number)
    return __builtin_clzll(number);
  return 64;
#elif defined(NQ_COMPILER_MSVC) && !defined(NQ_CPU_X86) && !(defined(NQ_CPU_32BIT) && defined(NQ_CPU_ARM))
  unsigned long ret = 0;
  if (_BitScanReverse64(&ret, number))
    return 63 - ret;
  return 64;
#else
  unsigned zeroCount = 0;
  for (int i = 63; i >= 0; i--) {
    if (!(number >> i))
      zeroCount++;
    else
      break;
  }
  return zeroCount;
#endif
}
