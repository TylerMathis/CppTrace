#ifndef IMAGE_H
#define IMAGE_H

#include "Vec3.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "stdint.h"
#include "stdlib.h"

#include <string>

struct Image {
  using Color3 = Color3<uint8_t>;

  const char *name;
  int width, height, comps, byte = 0;
  double aspectRatio;
  uint8_t *data;

  Image(const std::string name, const int width, const int height,
        const int comps)
      : name(name.c_str()), width(width), height(height), comps(comps),
        data((uint8_t *)calloc(comps * width * height, sizeof(uint8_t))),
        aspectRatio((double)width / height) {}

  void pushPixel(const Color3 &c) {
    for (int i = 0; i < 3; i++)
      data[byte + i] = c[i];
    byte += 3;
  }

  void write() {
    stbi_write_png(name, width, height, comps, data, width * comps);
  }
};

#endif