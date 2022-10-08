#ifndef HIT_H
#define HIT_H

#include "../Vec3.h"
#include "./Material/Material.h"

#include "float.h"

template <typename T> struct _Hit {
  using Vec3 = _Vec3<T>;
  using Point3 = _Point3<T>;
  using Material = _Material<T>;

  Point3 location;
  Vec3 normal;
  Material *material;
  double t;

  _Hit() : t(DBL_MAX) {}
  _Hit(Point3 &location, Vec3 &normal, Material *material, double t)
      : location(location), normal(normal), material(material), t(t) {}

  bool operator<(const _Hit &o) const { return t < o.t; }
};

#endif
