/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_ASSERT_H
#define _LIBNETQ_ASSERT_H

#include <libnetq/Basic.h>
#include <libnetq/Log.h>
#include <libnetq/Abort.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef NDEBUG

#define NQ_ASSERT_ENABLED 0
#define NQ_ASSERT_DISABLED 1

#define NQ_ASSERT(condition) ((void)0)
#define NQ_ASSERT_WITH_MESSAGE(condition, ...) ((void)0)
#define NQ_ASSERT_NOT_REACHED() ((void)0)
#define NQ_ASSERT_UNUSED(variable, condition) ((void)variable)

#define NQ_ALWAYS_ASSERT(condition) do { \
  if (NQ_UNLIKELY(!(condition))) { \
    NQAbort(); \
  } \
} while (0)
#define NQ_ALWAYS_ASSERT_WITH_MESSAGE(condition, ...) NQ_ALWAYS_ASSERT(condition)
#define NQ_ALWAYS_ASSERT_NOT_REACHED() NQAbort()

#else

#define NQ_ASSERT_ENABLED 1
#define NQ_ASSERT_DISABLED 0

#define NQ_ASSERT(condition) do { \
  if (!(condition)) { \
    NQLog_print(NQ_LOG_ERROR, NQ_LOG_TAG, "Assertion failed: %s", #condition); \
    NQLog_report(NQ_LOG_ERROR, NQ_LOG_TAG, NQ_FILE, NQ_LINE, NQ_PRETTY_FUNCTION); \
    NQAbort(); \
  } \
} while (0)

#define NQ_ASSERT_WITH_MESSAGE(condition, ...) do { \
  if (!(condition)) { \
    NQLog_print(NQ_LOG_ERROR, NQ_LOG_TAG, "Assertion failed: %s", #condition); \
    NQLog_print(NQ_LOG_ERROR, NQ_LOG_TAG, __VA_ARGS__); \
    NQLog_report(NQ_LOG_ERROR, NQ_LOG_TAG, NQ_FILE, NQ_LINE, NQ_PRETTY_FUNCTION); \
    NQAbort(); \
  } \
} while (0)

#define NQ_ASSERT_NOT_REACHED() do { \
  NQLog_print(NQ_LOG_ERROR, NQ_LOG_TAG, "Should not be reached"); \
  NQLog_report(NQ_LOG_ERROR, NQ_LOG_TAG, NQ_FILE, NQ_LINE, NQ_PRETTY_FUNCTION); \
  NQAbort(); \
} while (0)

#define NQ_ASSERT_UNUSED(variable, condition) NQ_ASSERT(condition)

#define NQ_ALWAYS_ASSERT(condition) NQ_ASSERT(condition)
#define NQ_ALWAYS_ASSERT_WITH_MESSAGE(condition, ...) NQ_ASSERT_WITH_MESSAGE(condition, __VA_ARGS__)
#define NQ_ALWAYS_ASSERT_NOT_REACHED() NQ_ASSERT_NOT_REACHED()

#endif /* NDEBUG */

#ifdef __cplusplus
#define NQ_STATIC_ASSERT static_assert
#else
#define _NQ_ASSERT_CONCAT2(a, b) a##b
#define _NQ_ASSERT_CONCAT(a, b) _NQ_ASSERT_CONCAT2(a, b)
#define NQ_STATIC_ASSERT(condition, message) \
  typedef int _NQ_ASSERT_CONCAT(static_assert_, NQ_COUNTER)[(condition) ? 1 : -1];
#endif

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_ASSERT_H */
