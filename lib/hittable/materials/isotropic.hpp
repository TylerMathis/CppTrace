//
// Created by Tyler Hostler-Mathis on 10/25/22.
//

#ifndef RAYTRACER_LIB_HITTABLE_MATERIALS_ISOTROPIC_HPP_
#define RAYTRACER_LIB_HITTABLE_MATERIALS_ISOTROPIC_HPP_

#include "material.hpp"
#include "../textures/texture.hpp"
#include "../textures/solid_color.hpp"
#include "../../common/vec3.hpp"
#include "../hit.hpp"

#include <memory>
#include <utility>

struct Isotropic : public Material {
  std::shared_ptr<Texture> texture;

  explicit Isotropic(const Color3 &color)
      : texture(std::make_shared<SolidColorTexture>(color)) {}

  explicit Isotropic(std::shared_ptr<Texture> texture)
      : texture(std::move(texture)) {}

  bool scatter(const Ray &in, const Hit &hit, Color3 &attenuation,
               Ray &out) const override {
    out = Ray(hit.location, Vec3::randomInUnitSphere());
    attenuation = texture->value(hit.u, hit.v, hit.location);
    return true;
  }
};

#endif //RAYTRACER_LIB_HITTABLE_MATERIALS_ISOTROPIC_HPP_
