#ifndef IMAGE_H
#define IMAGE_H

#include "Vec3.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "stdint.h"
#include "stdlib.h"

#include <string>

struct Image {
  const char *name;
  int width, height, comps, byte = 0;
  uint8_t *data;

  Image(const std::string name, const int width, const int height,
        const int comps)
      : name(name.c_str()), width(width), height(height), comps(comps),
        data((uint8_t *)calloc(comps * width * height, sizeof(uint8_t))) {}

  void pushPixel(Color3 c) {
    for (int i = 0; i < 3; i++)
      data[byte + i] = c[i];
    byte += 3;
  }

  void write() {
    stbi_write_png(name, width, height, comps, data, width * comps);
  }
};

#endif