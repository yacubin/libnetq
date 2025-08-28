/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/Version.h"

#include <libnetq/Limits.h>
#include <libnetq/Assert.h>

#define NQ_MAJOR_INDEX 0
#define NQ_MINOR_INDEX 1
#define NQ_PATCH_INDEX 2
#define NQ_TWEAK_INDEX 3

#define NQ_VERSION_UNDEFINED NQ_UINT32_MAX

void NQVersion_init(NQVersion* version)
{
  version->data[NQ_MAJOR_INDEX] = 0;
  version->data[NQ_MINOR_INDEX] = NQ_VERSION_UNDEFINED;
  version->data[NQ_PATCH_INDEX] = NQ_VERSION_UNDEFINED;
  version->data[NQ_TWEAK_INDEX] = NQ_VERSION_UNDEFINED;
}

void NQVersion_init1(NQVersion* version, uint32_t major)
{
  version->data[NQ_MAJOR_INDEX] = major;
  version->data[NQ_MINOR_INDEX] = NQ_VERSION_UNDEFINED;
  version->data[NQ_PATCH_INDEX] = NQ_VERSION_UNDEFINED;
  version->data[NQ_TWEAK_INDEX] = NQ_VERSION_UNDEFINED;
}

void NQVersion_init2(NQVersion* version, uint32_t major, uint32_t minor)
{
  version->data[NQ_MAJOR_INDEX] = major;
  version->data[NQ_MINOR_INDEX] = minor;
  version->data[NQ_PATCH_INDEX] = NQ_VERSION_UNDEFINED;
  version->data[NQ_TWEAK_INDEX] = NQ_VERSION_UNDEFINED;
}

void NQVersion_init3(NQVersion* version, uint32_t major, uint32_t minor, uint32_t patch)
{
  version->data[NQ_MAJOR_INDEX] = major;
  version->data[NQ_MINOR_INDEX] = minor;
  version->data[NQ_PATCH_INDEX] = patch;
  version->data[NQ_TWEAK_INDEX] = NQ_VERSION_UNDEFINED;
}

void NQVersion_init4(NQVersion* version, uint32_t major, uint32_t minor, uint32_t patch, uint32_t tweak)
{
  version->data[NQ_MAJOR_INDEX] = major;
  version->data[NQ_MINOR_INDEX] = minor;
  version->data[NQ_PATCH_INDEX] = patch;
  version->data[NQ_TWEAK_INDEX] = tweak;
}

static inline uint32_t NQVersion_getValue(const NQVersion* version, size_t index)
{
  NQ_ASSERT(version->data[index] != NQ_VERSION_UNDEFINED);
  return version->data[index];
}

static inline void NQVersion_setValue(NQVersion* version, size_t index, uint32_t value)
{
  NQ_ASSERT(value != NQ_VERSION_UNDEFINED);

  uint32_t* start = version->data + index;
  bool valid = (*start != NQ_VERSION_UNDEFINED);
  *start = value;

  if (valid)
    return;

  while (version->data != start) {
    if (*--start != NQ_VERSION_UNDEFINED)
      return;
    *start = 0;
  }
}

uint32_t NQVersion_major(const NQVersion* version)
{
  return NQVersion_getValue(version, NQ_MAJOR_INDEX);
}

void NQVersion_setMajor(NQVersion* version, uint32_t major)
{
  NQVersion_setValue(version, NQ_MAJOR_INDEX, major);
}

uint32_t NQVersion_minor(const NQVersion* version)
{
  return NQVersion_getValue(version, NQ_MINOR_INDEX);
}

void NQVersion_setMinor(NQVersion* version, uint32_t minor)
{
  NQVersion_setValue(version, NQ_MINOR_INDEX, minor);
}

uint32_t NQVersion_patch(const NQVersion* version)
{
  return NQVersion_getValue(version, NQ_PATCH_INDEX);
}

void NQVersion_setPatch(NQVersion* version, uint32_t patch)
{
  NQVersion_setValue(version, NQ_PATCH_INDEX, patch);
}

uint32_t NQVersion_tweak(const NQVersion* version)
{
  return NQVersion_getValue(version, NQ_TWEAK_INDEX);
}

void NQVersion_setTweak(NQVersion* version, uint32_t tweak)
{
  NQVersion_setValue(version, NQ_TWEAK_INDEX, tweak);
}

bool NQVersion_isZero(const NQVersion* version)
{
  size_t i;
  for (i = 0; i < 4; i++) {
    if (version->data[i] == NQ_VERSION_UNDEFINED)
      break;
    if (version->data[i] != 0)
      return false;
  }
  return true;
}

static NQ_ALWAYS_INLINE int NQVersion_cmp(const NQVersion* a, const NQVersion* b)
{
  size_t i;
  for (i = 0; i < 4; i++) {
    if (a->data[i] == NQ_VERSION_UNDEFINED) {
      if (b->data[i] == NQ_VERSION_UNDEFINED)
        return 0;
      if (b->data[i] != 0)
        return -1;
    }
    else if (b->data[i] == NQ_VERSION_UNDEFINED) {
      if (a->data[i] != 0)
        return 1;
    }
    else if (a->data[i] < b->data[i])
      return -1;
    else if (a->data[i] > b->data[i])
      return 1;
  }

  return 0;
}

bool NQVersion_less(const NQVersion* a, const NQVersion* b)
{
  return NQVersion_cmp(a, b) < 0;
}

bool NQVersion_greater(const NQVersion* a, const NQVersion* b)
{
  return NQVersion_cmp(a, b) > 0;
}

bool NQVersion_equal(const NQVersion* a, const NQVersion* b)
{
  return NQVersion_cmp(a, b) == 0;
}

bool NQVersion_lessEqual(const NQVersion* a, const NQVersion* b)
{
  return NQVersion_cmp(a, b) <= 0;
}

bool NQVersion_greaterEqual(const NQVersion* a, const NQVersion* b)
{
  return NQVersion_cmp(a, b) >= 0;
}
