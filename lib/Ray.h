#ifndef RAY_H
#define RAY_H

#include "Vec3.h"

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

#endif
