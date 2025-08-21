/*
 * MIT License
 *
 * Copyright (c) 2021-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/GetSineWave.h"

#include <math.h>

double NQSineWave_calc(const NQSineWave* thiz, double x)
{
  return NQGetSineWave(thiz->a, thiz->k, thiz->h, thiz->b, x);
}

double NQGetSineWave(double a, double k, double h, double b, double x)
{
  if (b == 0.0)
    return 0.0;

  return a * sin((x - h) / b) + k;
}
