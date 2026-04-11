/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_MODULE_H
#define _LIBNETQ_MODULE_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQSystem NQSystem;

#ifdef NQ_COMPILER_MSVC
# define NQ_USED
# define NQ_SECTION(name) __declspec(allocate(name))
# define NQ_USED_SECTION(name) __declspec(allocate(name))
#else
# define NQ_USED __attribute__((used))
# define NQ_SECTION(name) __attribute__((section(name)))
# define NQ_USED_SECTION(name) __attribute__((used, section(name)))
#endif

typedef int (*NQModuleInitCallback) (NQSystem*);
typedef void (*NQModuleExitCallback) (NQSystem*);

#define NQ_MODULE_INIT_NAME NQModuleInit
#define NQ_MODULE_INIT_STR  NQ_STRINGIZE_OF(NQ_MODULE_INIT_NAME)
#define NQ_MODULE_EXIT_NAME NQModuleExit
#define NQ_MODULE_EXIT_STR  NQ_STRINGIZE_OF(NQ_MODULE_EXIT_NAME)

#ifdef NQ_OS_WINDOWS

# define NQ_MODULE_INIT_SECTION "nqinit$m"
# define NQ_MODULE_EXIT_SECTION "nqexit$m"

# define NQ_MODULE_SECTIONS_DEFINE() \
  NQ_USED_SECTION("nqinit$a") NQModuleInitCallback __start_nqinit = 0; \
  NQ_USED_SECTION("nqinit$z") NQModuleInitCallback __end_nqinit = 0;   \
  NQ_USED_SECTION("nqexit$a") NQModuleExitCallback __start_nqexit = 0; \
  NQ_USED_SECTION("nqexit$z") NQModuleExitCallback __end_nqexit = 0

# define NQ_SECTION_FOREACH(type, name) \
  for (type* iter = &__start_##name + 1; iter < &__end_##name; iter++)

#else

# define NQ_MODULE_INIT_SECTION "nqinit"
# define NQ_MODULE_EXIT_SECTION "nqexit"

# define NQ_MODULE_SECTIONS_DEFINE() \
  extern NQModuleInitCallback __start_nqinit[]; \
  extern NQModuleInitCallback __stop_nqinit[];  \
  extern NQModuleExitCallback __start_nqexit[]; \
  extern NQModuleExitCallback __stop_nqexit[]

# define NQ_SECTION_FOREACH(type, name) \
  for (type* iter = __start_##name; iter < __stop_##name; iter++)

#endif

#ifndef NQ_MODULE_INTERNAL

#define NQ_MODULE_INIT(fn) \
  NQ_EXPORT_DECLARATION int NQ_MODULE_INIT_NAME(NQSystem* system) \
  { \
    return fn(system); \
  }

#define NQ_MODULE_EXIT(fn) \
  NQ_EXPORT_DECLARATION void NQ_MODULE_EXIT_NAME(NQSystem* system) \
  { \
    fn(system); \
  }

#else

#if defined(NQ_COMPILER_MSVC)
#pragma section("nqinit$a", read)
#pragma section(NQ_MODULE_INIT_SECTION, read)
#pragma section("nqinit$z", read)
#pragma section("nqexit$a", read)
#pragma section(NQ_MODULE_EXIT_SECTION, read)
#pragma section("nqexit$z", read)
#endif

#define NQ_MODULE_INIT(fn) \
  NQ_USED_SECTION(NQ_MODULE_INIT_SECTION)          \
  NQModuleInitCallback NQ_CONCAT(nqinit_,          \
                       NQ_CONCAT(__COUNTER__,      \
                       NQ_CONCAT(_,                \
                       NQ_CONCAT(__LINE__,         \
                       NQ_CONCAT(_, fn))))) = fn

#define NQ_MODULE_EXIT(fn) \
  NQ_USED_SECTION(NQ_MODULE_EXIT_SECTION)          \
  NQModuleExitCallback NQ_CONCAT(nqexit_,          \
                       NQ_CONCAT(__COUNTER__,      \
                       NQ_CONCAT(_,                \
                       NQ_CONCAT(__LINE__,         \
                       NQ_CONCAT(_, fn))))) = fn

#endif

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_MODULE_H */
