//
// Created by Tyler Hostler-Mathis on 10/18/22.
//

#ifndef RAYTRACER_LIB_HITTABLE_MATERIAL_LAMBERTIAN_HPP_
#define RAYTRACER_LIB_HITTABLE_MATERIAL_LAMBERTIAN_HPP_

#include "../../common/ray.hpp"
#include "../../common/vec3.hpp"
#include "../hit.hpp"
#include "material.hpp"
#include "../textures/texture.hpp"

#include <memory>
#include <utility>

struct Lambertian : public Material {
  std::shared_ptr<Texture> texture;

  Lambertian() = default;
  explicit Lambertian(std::shared_ptr<Texture> texture)
      : texture(std::move(texture)) {}

  bool scatter(const Ray &in, const Hit &hit, Color3 &attenuation,
               Ray &out) const override {
    Point3 target = hit.location + Vec3::randomInHemisphere(hit.normal);
    out = Ray(hit.location, target - hit.location);
    attenuation = texture->value(hit.u, hit.v, hit.location);
    return true;
  }
};

#endif //RAYTRACER_LIB_HITTABLE_MATERIAL_LAMBERTIAN_HPP_
