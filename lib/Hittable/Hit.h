#ifndef HIT_H
#define HIT_H

#include "../Vec3.h"
#include "./Material/Material.h"

#include <cfloat>
#include <memory>
#include <utility>

struct Hit {
  Point3 location;
  Vec3 normal;
  std::shared_ptr<Material> material;
  double t;
  bool front{};

  Hit() : t(DBL_MAX) {}
  Hit(const Point3 &location, const Vec3 &normal, std::shared_ptr<Material> material,
      double t, bool front)
      : location(location), normal(normal), material(std::move(material)), t(t),
        front(front) {}

  bool operator<(const Hit &o) const { return t < o.t; }
};

#endif
