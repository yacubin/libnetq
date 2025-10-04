/*
 * MIT License
 *
 * Copyright (c) 2023-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_IMAGE_BUILDER_H
#define _LIBNETQ_IMAGE_BUILDER_H

#include <libnetq/Basic.h>
#include <libnetq/ColorSpace.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQImageBuilder NQImageBuilder;
struct NQImageBuilder {
  NQColorSpace colorspace;
  uint32_t width;
  uint32_t height;
  uint32_t stride;
  uint8_t* data;
};

NQImageBuilder* NQImageBuilder_create(NQColorSpace colorspace, uint32_t width, uint32_t height);
void NQImageBuilder_destroy(NQImageBuilder*);

bool NQImageBuilder_addLine(NQImageBuilder*, const uint8_t* line);
bool NQImageBuilder_finish(NQImageBuilder*);

typedef struct NQYUVImageBuilder NQYUVImageBuilder;
struct NQYUVImageBuilder {
  uint32_t width;
  uint32_t height;

  uint8_t* dataY;
  uint8_t* dataU;
  uint8_t* dataV;

  uint32_t sizeY;
  uint32_t sizeU;
  uint32_t sizeV;

  uint32_t strideY;
  uint32_t strideU;
  uint32_t strideV;
};

NQYUVImageBuilder* NQYUVImageBuilder_create(uint32_t width, uint32_t height);
void NQYUVImageBuilder_destroy(NQYUVImageBuilder*);

bool NQYUVImageBuilder_addLine(NQYUVImageBuilder*, const uint8_t* line);
bool NQYUVImageBuilder_finish(NQYUVImageBuilder*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_IMAGE_BUILDER_H */
