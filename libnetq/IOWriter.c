/*
 * MIT License
 *
 * Copyright (c) 2025-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/IOWriter.h"

int NQIOWriter_flushStub(NQIOWriter* thiz)
{
  NQ_UNUSED_PARAM(thiz);
  return 0;
}

void NQIOWriter_releaseStub(NQIOWriter* thiz)
{
  NQ_UNUSED_PARAM(thiz);
}
