/*
 * MIT License
 *
 * Copyright (c) 2021-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/CType.h"

#include <libnetq/String.h>

bool NQIsFalse(const char* s)
{
  return nq_stricmp(s, NQ_FALSE_STRING) == 0;
}

bool NQIsTrue(const char* s)
{
  return nq_stricmp(s, NQ_TRUE_STRING) == 0;
}

bool NQIsOff(const char* s)
{
  return nq_stricmp(s, NQ_OFF_STRING) == 0;
}

bool NQIsOn(const char* s)
{
  return nq_stricmp(s, NQ_ON_STRING) == 0;
}

bool NQIsEnabled(const char* s)
{
  if (NQIsTrue(s) || NQIsOn(s))
    return true;
  if (!nq_stricmp(s, NQ_ONE_STRING))
    return true;

  return false;
}

bool NQIsDisabled(const char* s)
{
  if (NQIsFalse(s) || NQIsOff(s))
    return true;
  if (!nq_stricmp(s, NQ_ZERO_STRING))
    return true;

  return false;
}
