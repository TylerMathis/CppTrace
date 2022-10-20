//
// Created by Tyler Hostler-Mathis on 10/18/22.
//

#ifndef RAYTRACER_LIB_HITTABLE_MATERIAL_METAL_HPP_
#define RAYTRACER_LIB_HITTABLE_MATERIAL_METAL_HPP_

#include "../../common/common.hpp"
#include "../../common/ray.hpp"
#include "../../common/vec3.hpp"
#include "../hit.hpp"
#include "material.hpp"

struct Metal : public Material {
  Color3 albedo;
  double fuzziness;

  Metal() : albedo(), fuzziness(0) {}
  explicit Metal(const Color3 &albedo, double fuzziness = 0)
      : albedo(albedo), fuzziness(common::clamp(fuzziness, 0, 1)) {}

  bool scatter(const Ray &in, const Hit &hit, Color3 &attenuation,
               Ray &out) const override {
    Vec3 reflected = in.direction.unit().reflect(hit.normal);
    out = Ray(hit.location, reflected + Vec3::randomInUnitSphere() * fuzziness);
    attenuation = albedo;
    return true;
  }
};

#endif //RAYTRACER_LIB_HITTABLE_MATERIAL_METAL_HPP_
