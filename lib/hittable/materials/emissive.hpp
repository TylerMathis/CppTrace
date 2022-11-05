//
// Created by Tyler Hostler-Mathis on 10/19/22.
//

#ifndef CPPTRACE_LIB_HITTABLE_MATERIAL_EMISSIVE_HPP_
#define CPPTRACE_LIB_HITTABLE_MATERIAL_EMISSIVE_HPP_

#include "material.hpp"
#include "../textures/texture.hpp"

#include <memory>
#include <utility>

struct Emissive : public Material {
  std::shared_ptr<Texture> texture;

  Emissive() = default;
  explicit Emissive(std::shared_ptr<Texture> texture)
      : texture(std::move(texture)) {}

  bool scatter(const Ray &in, const Hit &hit, Color3 &attenuation,
               Ray &out) const override {
    return false;
  }

  [[nodiscard]] Color3 emit(const double u,
                            const double v,
                            const Point3 &point) const override {
    return texture->value(u, v, point);
  }
};

#endif //CPPTRACE_LIB_HITTABLE_MATERIAL_EMISSIVE_HPP_
