#ifndef HIT_H
#define HIT_H

#include "../Vec3.h"
#include "./Material/Material.h"

#include "float.h"

#include <memory>

template <typename T> struct _Hit {
  Point3 location;
  Vec3 normal;
  std::shared_ptr<Material> material;
  double t;
  bool front;

  _Hit() : t(DBL_MAX) {}
  _Hit(Point3 &location, Vec3 &normal, std::shared_ptr<Material> material,
       double t, bool front)
      : location(location), normal(normal), material(material), t(t),
        front(front) {}

  bool operator<(const _Hit &o) const { return t < o.t; }
};

using Hit = _Hit<double>;

#endif
