#ifndef IMAGE_H
#define IMAGE_H

#include "Vec3.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "Common.h"

#include "stdint.h"
#include "stdlib.h"

#include <iostream>
#include <string>

struct Image {
  using Color3 = _Color3<double>;

  std::string name;
  int width, height, comps, samples, bounces;
  double aspectRatio;
  uint8_t *data;

  Image(const std::string name, const int width, const int height,
        const int comps, const int samples = 50, const int bounces = 50)
      : name(getPath(name)), width(width), height(height), comps(comps),
        samples(samples), bounces(bounces),
        data((uint8_t *)calloc(comps * width * height, sizeof(uint8_t))),
        aspectRatio((double)width / height) {}

  std::string getPath(const std::string name) {
    return "./images/" + name + ".png";
  }
  void setName(const std::string newName) { name = getPath(newName); }

  void setPixel(const int row, const int col, const Color3 &c) {
    int byte = (row * width + col) * 3;
    Color3 sampleAndGammaCorrected = (c * (1.0 / samples)).sqrt();
    for (int i = 0; i < 3; i++)
      data[byte + i] = static_cast<int>(
          256 * common::clamp(sampleAndGammaCorrected[i], 0, 0.999));
  }

  void write() {
    std::cerr << "Writing image to " << name << "\n";
    stbi_write_png(name.c_str(), width, height, comps, data, width * comps);
  }
};

#endif
