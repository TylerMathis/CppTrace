#ifndef HIT_H
#define HIT_H

#include "Vec3.h"

#include "float.h"

template <typename T> struct Hit {
  using Vec3 = Vec3<T>;

  Vec3 normal;
  double dist;

  Hit() : dist(DBL_MAX) {}
  Hit(Vec3 &normal, double &dist) : normal(normal), dist(dist) {}

  bool operator<(const Hit &o) const { return dist < o.dist; }
};

#endif