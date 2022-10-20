//
// Created by Tyler Hostler-Mathis on 10/18/22.
//

#ifndef RAYTRACER_LIB_SCENE_IMAGE_HPP_
#define RAYTRACER_LIB_SCENE_IMAGE_HPP_

#include "../common/common.hpp"
#include "../common/vec3.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../extern/stb_image_write.hpp"

#include <cstdint>
#include <cstdlib>
#include <cfloat>
#include <iostream>
#include <string>

struct Image {
  std::string name;
  int width, height, samples, bounces;
  double minRayLength, maxRayLength;
  double aspectRatio;
  int comps = 3;
  uint8_t *data;

  Image(const std::string &name,
        const int width,
        const int height,
        const int samples = 50,
        const int bounces = 50,
        const double minRayLength = 0.001,
        const double maxRayLength = DBL_MAX)
      : name(getPath(name)),
        width(width),
        height(height),
        samples(samples),
        bounces(bounces),
        data((uint8_t *) calloc(comps * width * height, sizeof(uint8_t))),
        aspectRatio((double) width / height),
        minRayLength(minRayLength),
        maxRayLength(maxRayLength) {}

  static std::string getPath(const std::string &name) {
    return "./images/" + name + ".png";
  }
  void setName(const std::string &newName) { name = getPath(newName); }

  void setPixel(const int row, const int col, const Color3 &c) const {
    int byte = (row * width + col) * 3;
    Color3 sampleAndGammaCorrected = (c * (1.0 / samples)).sqrt();
    for (int i = 0; i < 3; i++)
      data[byte + i] = static_cast<int>(
          256 * common::clamp(sampleAndGammaCorrected[i], 0, 0.999));
  }

  void write() const {
    std::cout << "Writing image to " << name << "\n";
    stbi_write_png(name.c_str(), width, height, comps, data, width * comps);
  }
};

#endif //RAYTRACER_LIB_SCENE_IMAGE_HPP_
