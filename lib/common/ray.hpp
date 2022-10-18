//
// Created by Tyler Hostler-Mathis on 10/18/22.
//

#ifndef RAYTRACER_LIB_COMMON_RAY_HPP_
#define RAYTRACER_LIB_COMMON_RAY_HPP_

#include "vec3.hpp"

struct Ray {
  Point3 origin;
  Vec3 direction;

  Ray() = default;
  Ray(const Point3 &origin, const Vec3 &direction)
      : origin(origin), direction(direction) {}

  [[nodiscard]] Point3 at(const double t) const {
    return origin + direction * t;
  }
};

#endif //RAYTRACER_LIB_COMMON_RAY_HPP_
