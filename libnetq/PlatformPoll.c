/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/PlatformPoll.h"

#ifdef NQ_OS_WINDOWS
int NQPlatformPoll(NQPlatformPollfd* fds, unsigned long nfds, int timeout)
{
  return WSAPoll(fds, nfds, timeout);
}
#endif
