//
// Created by Tyler Hostler-Mathis on 10/24/22.
//

#ifndef RAYTRACER_LIB_HITTABLE_TEXTURES_SOLID_COLOR_HPP_
#define RAYTRACER_LIB_HITTABLE_TEXTURES_SOLID_COLOR_HPP_

#include "texture.hpp"
#include "../../common/vec3.hpp"

struct SolidColor : public Texture {
  Color3 color;

  SolidColor() = default;
  explicit SolidColor(const Color3 &color) : color(color) {}
  SolidColor(const double r, const double g, const double b)
      : SolidColor(Color3(r, g, b)) {}

  [[nodiscard]] Color3 value(const double u,
                             const double v,
                             const Vec3 &point) const override {
    return color;
  }
};

#endif //RAYTRACER_LIB_HITTABLE_TEXTURES_SOLID_COLOR_HPP_
