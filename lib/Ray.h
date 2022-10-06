#ifndef RAY_H
#define RAY_H

#include "Vec3.h"

struct Ray {
  Point3 orig;
  Vec3 dir;

  Ray() {}
  Ray(const Point3 &orig, const Vec3 &dir) : orig(orig), dir(dir) {}

  Point3 at(const double t) const { return orig + dir * t; }
};

#endif