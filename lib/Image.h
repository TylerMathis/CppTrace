#ifndef IMAGE
#define IMAGE

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
  Image(const std::string _name, const int _width, const int _height,
        const int _comps) {
    name = _name.c_str();
    width = _width, height = _height, comps = _comps;
    data = (uint8_t *)calloc(comps * width * height, sizeof(uint8_t));
  }
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