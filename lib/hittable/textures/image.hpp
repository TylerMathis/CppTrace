//
// Created by Tyler Hostler-Mathis on 10/24/22.
//

#ifndef CPPTRACE_LIB_HITTABLE_TEXTURES_IMAGE_HPP_
#define CPPTRACE_LIB_HITTABLE_TEXTURES_IMAGE_HPP_

#include "texture.hpp"
#include "../../common/vec3.hpp"
#include "../../common/common.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../../extern/stb_image_read.hpp"

#include <string>
#include <iostream>

struct ImageTexture : public Texture {
  unsigned char *data{};
  int width{}, height{};
  int bytesPerScanline{}, bytesPerPixel = 3;

  ImageTexture() = default;
  explicit ImageTexture(const std::string &filepath) {

    int componentsFound;
    data =
        stbi_load(filepath.c_str(),
                  &width,
                  &height,
                  &componentsFound,
                  bytesPerPixel);

    if (!data) {
      std::cerr << "Error: Texture path " << filepath
                << " is not a valid path\n";
      width = height = 0;
      return;
    }
    std::cout << "Loaded texture " << filepath << "\n";
    bytesPerScanline = bytesPerPixel * width;
  }

  ~ImageTexture() { delete data; }

  [[nodiscard]] Color3 value(double u,
                             double v,
                             const Vec3 &point) const override {
    // If no data, PINK
    if (data == nullptr)
      return {1, 0.7, 0.75};

    // Clamp u and v into valid space
    u = common::clamp(u, 0.0, 1.0);
    v = 1.0 - common::clamp(v, 0.0, 1.0);

    // Convert u and v into their respective indices
    auto i = (int) (u * width);
    auto j = (int) (v * height);

    // Again, clamp i and j
    if (i >= width) i = width - 1;
    if (j >= height) j = height - 1;

    int dataIndex = j * bytesPerScanline + i * bytesPerPixel;
    return Color3(data[dataIndex + 0], data[dataIndex + 1], data[dataIndex + 2])
        / 255.0;
  }
};

#endif //CPPTRACE_LIB_HITTABLE_TEXTURES_IMAGE_HPP_
