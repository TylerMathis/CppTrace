#ifndef RAY_H
#define RAY_H

#include "Vec3.h"

template <typename T> struct Ray {
  using Vec3 = Vec3<T>;
  using Point3 = Point3<T>;

  Point3 origin;
  Vec3 direction;

  Ray() {}
  Ray(const Point3 &origin, const Vec3 &direction)
      : origin(origin), direction(direction) {}

  Point3 at(const double t) const { return origin + direction * t; }
};

#endif