/*
 * MIT License
 *
 * Copyright (c) 2021-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_SIGNAL_H
#define _LIBNETQ_SIGNAL_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

enum NQSignalType {
  // Windows
  kNQSignalTypeCtrlC,
  kNQSignalTypeCtrlBreak,
  kNQSignalTypeCtrlClose,
  kNQSignalTypeCtrlLogoff,
  kNQSignalTypeCtrlShutdown,
  // Unix
  kNQSignalTypeSigInt,
  kNQSignalTypeSigTerm,
  kNQSignalTypeSigKill,
  kNQSignalTypeSigSegv,
};

typedef void (*NQSignalHandler) (int type, void*);

NQ_EXPORT bool NQSetSignalHandler(int type, NQSignalHandler handler, void* userdata);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_SIGNAL_H */
