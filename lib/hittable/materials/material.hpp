//
// Created by Tyler Hostler-Mathis on 10/18/22.
//

#ifndef CPPTRACE_LIB_HITTABLE_MATERIAL_MATERIAL_HPP_
#define CPPTRACE_LIB_HITTABLE_MATERIAL_MATERIAL_HPP_

#include "../../common/ray.hpp"
#include "../../common/vec3.hpp"

// Forward declaration to avoid dependency cycle
struct Hit;

struct Material {
  virtual bool scatter(const Ray &in, const Hit &hit, Color3 &attenuation,
                       Ray &out) const = 0;

  [[nodiscard]] virtual Color3 emit(const double u,
                                    const double v,
                                    const Point3 &point) const {
    return {};
  }
};

#endif //CPPTRACE_LIB_HITTABLE_MATERIAL_MATERIAL_HPP_
