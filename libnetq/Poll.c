/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/Poll.h"

#include <libnetq/Assert.h>

#ifdef NQ_OS_WIN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#else
#include <poll.h>
#include <errno.h>
#endif

uint16_t NQPollEventsToPlatform(uint16_t events)
{
  uint16_t result = 0;

  if (events & NQ_POLLIN)
    result |= POLLIN;
  
  if (events & NQ_POLLPRI)
    result |= POLLPRI;

  if (events & NQ_POLLOUT)
    result |= POLLOUT;

  if (events & NQ_POLLRDNORM)
    result |= POLLRDNORM;

  if (events & NQ_POLLRDBAND)
    result |= POLLRDBAND;

  if (events & NQ_POLLWRNORM)
    result |= POLLWRNORM;

  if (events & NQ_POLLWRBAND)
    result |= POLLWRBAND;

  if (events & NQ_POLLERR)
    result |= POLLERR;

  if (events & NQ_POLLHUP)
    result |= POLLHUP;

  if (events & NQ_POLLNVAL)
    result |= POLLNVAL;

  return result;
}

uint16_t NQPollEventsFromPlatform(uint16_t events)
{
  uint16_t result = 0;

  if (events & POLLIN)
    result |= NQ_POLLIN;

  if (events & POLLPRI)
    result |= NQ_POLLPRI;

  if (events & POLLOUT)
    result |= NQ_POLLOUT;

  if (events & POLLRDNORM)
    result |= NQ_POLLRDNORM;

  if (events & POLLRDBAND)
    result |= NQ_POLLRDBAND;

  if (events & POLLWRNORM)
    result |= NQ_POLLWRNORM;

  if (events & POLLWRBAND)
    result |= NQ_POLLWRBAND;

  if (events & POLLERR)
    result |= NQ_POLLERR;

  if (events & POLLHUP)
    result |= NQ_POLLHUP;

  if (events & POLLNVAL)
    result |= NQ_POLLNVAL;

  return result;
}

int NQPollWait(NQPollSocket* data, size_t size, int64_t timeout)
{
  NQ_STATIC_ASSERT(sizeof(*data) == sizeof(struct pollfd), "Size pollfd wrong");

  int ret;
  size_t i;

  for (i = 0; i < size; i++) {
    data[i].events = NQPollEventsToPlatform(data[i].events);
    NQ_ASSERT(data[i].revents == 0);
  }

#ifdef NQ_OS_WIN
  ret = WSAPoll((LPWSAPOLLFD)data, (ULONG)size,  (INT)timeout);
#else
  do {
    ret = poll((struct pollfd*)data, (nfds_t)size, (int)timeout);
  } while(ret == -1 && errno == EINTR);
#endif

  for (i = 0; i < size; i++) {
    data[i].events = NQPollEventsFromPlatform(data[i].events);
    data[i].revents = NQPollEventsFromPlatform(data[i].revents);
  }

  return ret;
}
