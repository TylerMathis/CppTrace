//
// Created by Tyler Hostler-Mathis on 10/19/22.
//

#ifndef RAYTRACER_LIB_HITTABLE_MATERIAL_EMISSIVE_HPP_
#define RAYTRACER_LIB_HITTABLE_MATERIAL_EMISSIVE_HPP_

#include "material.hpp"

struct Emissive : public Material {
  Color3 emission;

  Emissive() : emission() {}
  explicit Emissive(const Color3 &emission) : emission(emission) {}

  bool scatter(const Ray &in, const Hit &hit, Color3 &attenuation,
               Ray &out) const override {
    return false;
  }

  [[nodiscard]] Color3 emit(const double u,
                            const double v,
                            const Point3 &point) const override {
    return emission;
  }
};

#endif //RAYTRACER_LIB_HITTABLE_MATERIAL_EMISSIVE_HPP_
