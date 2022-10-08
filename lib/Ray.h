#ifndef RAY_H
#define RAY_H

#include "Vec3.h"

template <typename T> struct _Ray {
  using Vec3 = _Vec3<T>;
  using Point3 = _Point3<T>;

  Point3 origin;
  Vec3 direction;

  _Ray() {}
  _Ray(const Point3 &origin, const Vec3 &direction)
      : origin(origin), direction(direction) {}

  Point3 at(const double t) const { return origin + direction * t; }
};

#endif
