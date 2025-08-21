/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/BuildInfo.h"

#include <libnetq/CPU.h>
#include <libnetq/Compiler.h>

const char* NQGetBuildABI()
{
#ifdef NQ_CPU_ABI
  return NQ_CPU_ABI;
#else
  return "unknown";
#endif
}

const char* NQGetBuildDateTime()
{
  return NQ_DATETIME;
}
