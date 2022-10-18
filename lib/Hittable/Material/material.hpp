//
// Created by Tyler Hostler-Mathis on 10/18/22.
//

#ifndef RAYTRACER_LIB_HITTABLE_MATERIAL_MATERIAL_HPP_
#define RAYTRACER_LIB_HITTABLE_MATERIAL_MATERIAL_HPP_

#include "../../common/ray.hpp"
#include "../../common/vec3.hpp"

// Forward declaration to avoid dependency cycle
struct Hit;

struct Material {
  virtual void scatter(const Ray &in, const Hit &hit, Color3 &attenuation,
                       Ray &out) const = 0;
};

#endif //RAYTRACER_LIB_HITTABLE_MATERIAL_MATERIAL_HPP_
