//
// Created by Tyler Hostler-Mathis on 10/18/22.
//

#ifndef RAYTRACER_LIB_HITTABLE_MATERIAL_LAMBERTIAN_HPP_
#define RAYTRACER_LIB_HITTABLE_MATERIAL_LAMBERTIAN_HPP_

#include "../../common/ray.hpp"
#include "../../common/vec3.hpp"
#include "../hit.hpp"
#include "material.hpp"

struct Lambertian : public Material {
  Color3 albedo;

  Lambertian() = default;
  explicit Lambertian(const Color3 &albedo) : albedo(albedo) {}

  void scatter(const Ray &in, const Hit &hit, Color3 &attenuation,
               Ray &out) const override {
    Point3 target = hit.location + Vec3::randomInHemisphere(hit.normal);
    out = Ray(hit.location, target - hit.location);
    attenuation = albedo;
  }
};

#endif //RAYTRACER_LIB_HITTABLE_MATERIAL_LAMBERTIAN_HPP_
