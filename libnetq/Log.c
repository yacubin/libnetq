/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/Log.h"

#include <libnetq/CStrBase.h>
#include <libnetq/Sprintf.h>
#include <libnetq/Math.h>
#include <libnetq/Time.h>
#include <libnetq/Abort.h>
#include <libnetq/Assert.h>

#ifdef NQ_OS_ANDROID
#include <android/log.h>
#endif

#ifdef NQ_OS_WINDOWS
#include <windows.h>
#endif

#ifdef NQ_COMPILER_MSVC
#include <crtdbg.h>
#endif

#define BUFFER_SIZE 1024

char NQLogLevelToChar(NQLogLevel level)
{
  switch (level) {
  case NQ_LOG_INFO:
    return 'I';

  case NQ_LOG_WARNING:
    return 'W';

  case NQ_LOG_ERROR:
    return 'E';

  case NQ_LOG_DEBUG:
    return 'D';

  default:
    NQ_ASSERT_NOT_REACHED();
    return '?';

  };
}

int NQLog_snprint(char* buffer, size_t size, NQLogLevel level, const char* tag, const char* format, ...)
{
  int result;
  va_list args;
  va_start(args, format);
  result = NQLog_vsnprint(buffer, size, level, tag, format, args);
  va_end(args);
  return result;
}

int NQLog_vsnprint(char* buffer, size_t size, NQLogLevel level, const char* tag, const char* format, va_list args)
{
  int n;
  char* ptr;
  char* end;
  size_t result;
  size_t len;

  ptr = buffer;
  end = ptr + size;

  result = NQTimeFormat(NQGetTime(), NQ_DT_RFC3339, buffer, size);
  ptr += result;

  if (ptr < end)
    *ptr = ' ';
  ptr++;

  if (ptr < end)
    *ptr = NQLogLevelToChar(level);
  ptr++;

  if (ptr < end)
    *ptr = ' ';
  ptr++;

  len = strlen(tag);
  if (ptr < end)
    memcpy(ptr, tag, NQGetMin((size_t)(end - ptr), len));
  ptr += len;

  if (ptr < end)
    *ptr = ' ';
  ptr++;
  
  len = ptr < end ? end - ptr : 0;
  n = vsnprintf(ptr, len, format, args);
  ptr += n;

  return ptr - buffer;
}

int NQLog_print(NQLogLevel level, const char* tag, const char* format, ...)
{
  int result;
  va_list args;
  va_start(args, format);
  result = NQLog_vprint(level, tag, format, args);
  va_end(args);
  return result;
}

int NQLog_vprint(NQLogLevel level, const char* tag, const char* format, va_list args)
{
#if defined(NQ_SYS_LINUX)
  return vprintk(format, args);

#elif defined(NQ_OS_ANDROID)
  // adb logcat -s "NQ"
  int result;
  int prio;

  switch (level) {
  case NQ_LOG_INFO:
    prio = ANDROID_LOG_INFO;
    break;
  case NQ_LOG_WARNING:
    prio = ANDROID_LOG_WARN;
    break;
  case NQ_LOG_ERROR:
    prio = ANDROID_LOG_ERROR;
    break;
  case NQ_LOG_DEBUG:
    prio = ANDROID_LOG_DEBUG;
    break;
  default:
    prio = ANDROID_LOG_ERROR;
    NQ_ASSERT_NOT_REACHED();
    break;
  }

  result = __android_log_vprint(prio, tag, format, args);
  return result;
#else
  size_t result;
  char buffer[BUFFER_SIZE];
  result = NQLog_vsnprint(buffer, sizeof(buffer), level, tag, format, args);

  char* ptr;
  if (result + 2 <= sizeof(buffer))
    ptr = buffer + result;
  else
    ptr = buffer + sizeof(buffer) - 2;

#ifdef NQ_COMPILER_MSVC
  *ptr++ = '\0';
  OutputDebugStringA(buffer);
#else
  *ptr++ = '\n';
  *ptr++ = '\0';
  fputs(buffer, stderr);
#endif

  return result;
#endif
}

void NQLog_report(NQLogLevel level, const char* tag, const char* file, int line, const char* function)
{
#if defined(_WIN32) && defined(_DEBUG)
  _CrtDbgReport(_CRT_WARN, file, line, NULL, "%s\n", function);
#else
  NQLog_print(level, tag, "%s(%d) : %s", file, line, function);
#endif
}

void NQLog_assert(const char* condition, const char* tag, const char* format, ...)
{
  if (condition)
    NQLog_print(NQ_LOG_ERROR, tag, "Assertion failed: %s", condition);
  else
    NQLog_print(NQ_LOG_ERROR, tag, "Should not be reached");

  if (format) {
    va_list args;
    va_start(args, format);
    NQLog_vprint(NQ_LOG_ERROR, tag, format, args);
    va_end(args);
  }

  NQAbort();
}
