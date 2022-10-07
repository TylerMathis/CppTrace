#ifndef HIT_H
#define HIT_H

#include "../Vec3.h"
#include "./Material/Material.h"

#include "float.h"

template <typename T> struct Hit {
  using Vec3 = Vec3<T>;
  using Point3 = Point3<T>;
  using Material = Material<T>;

  Point3 location;
  Vec3 normal;
  Material *material;
  double t;

  Hit() : t(DBL_MAX) {}
  Hit(Point3 &location, Vec3 &normal, Material *material, double t)
      : location(location), normal(normal), material(material), t(t) {}

  bool operator<(const Hit &o) const { return t < o.t; }
};

#endif