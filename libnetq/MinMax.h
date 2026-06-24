/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_MINMAX_H
#define _LIBNETQ_MINMAX_H

#define NQGetMin(a, b) ((a) > (b) ? (b) : (a))
#define NQGetMax(a, b) ((a) < (b) ? (b) : (a))
#define NQGetClamp(_value, _min, _max) (_value > _min ? (_value < _max ? _value : _max) : _min)
#define NQGetDiff(a, b) (NQGetMax(a, b) - NQGetMin(a, b))

#endif /* _LIBNETQ_MINMAX_H */
