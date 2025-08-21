/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_EXPORT_H
#define _LIBNETQ_EXPORT_H

#include <libnetq/OS.h>

#ifdef __cplusplus
# define NQ_EXTERN_C extern "C"
# define NQ_EXTERN_C_BEGIN NQ_EXTERN_C {
# define NQ_EXTERN_C_END }
#else
# define NQ_EXTERN_C
# define NQ_EXTERN_C_BEGIN
# define NQ_EXTERN_C_END
#endif

#ifdef NQ_OS_WINDOWS
# define NQ_EXPORT_DECLARATION __declspec(dllexport)
# define NQ_IMPORT_DECLARATION __declspec(dllimport)
#elif defined(__GNUC__)
# define NQ_EXPORT_DECLARATION __attribute__((visibility("default")))
# define NQ_IMPORT_DECLARATION NQ_EXPORT_DECLARATION
#else
# define NQ_EXPORT_DECLARATION
# define NQ_IMPORT_DECLARATION
#endif

#ifdef NQ_USE_EXPORT
# ifdef NQ_INTERNAL
#  define NQ_EXPORT NQ_EXPORT_DECLARATION
# else
#  define NQ_EXPORT NQ_IMPORT_DECLARATION
# endif
#else
# define NQ_EXPORT
#endif

#endif /* _LIBNETQ_EXPORT_H */
