/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/CPU.h"

#ifdef NQ_OS_WINDOWS
#include <windows.h>
#endif

#ifdef NQ_OS_DARWIN
#include <sys/sysctl.h>
#endif

#ifdef NQ_OS_LINUX
#include <unistd.h>
#endif

#ifdef NQ_SYS_LINUX
#include <linux/cpumask.h>
#endif

static unsigned s_numberOfProcessors = 0;

static inline unsigned getOSNumberOfCores(void)
{
#if defined(NQ_OS_WINDOWS)
  SYSTEM_INFO sysInfo;
  GetSystemInfo(&sysInfo);
  return (unsigned)sysInfo.dwNumberOfProcessors;
  
#elif defined(NQ_OS_DARWIN)
  unsigned value;
  size_t length = sizeof(value);
  int names[] = { CTL_HW, HW_AVAILCPU };
  int ret = sysctl(names, NQ_ARRAY_LENGTH(names), &value, &length, 0, 0);
  return (ret < 0) ? 0 : value;

#elif defined(NQ_OS_LINUX)
  long ret = sysconf(_SC_NPROCESSORS_ONLN);
  return (ret < 0) ? 0 : (unsigned)(ret);

#elif defined(NQ_SYS_LINUX)
  return num_online_cpus();

#else
  return 0;

#endif
}

unsigned NQGetNumberOfProcessors(void)
{
  if (s_numberOfProcessors == 0) {
    unsigned num = getOSNumberOfCores();
    s_numberOfProcessors = num ? num : 1;
  }
  
  return s_numberOfProcessors;  
}
