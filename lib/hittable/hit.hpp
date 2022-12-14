//
// Created by Tyler Hostler-Mathis on 10/18/22.
//

#ifndef CPPTRACE_LIB_HITTABLE_HIT_HPP_
#define CPPTRACE_LIB_HITTABLE_HIT_HPP_

#include "../common/vec3.hpp"
#include "materials/material.hpp"

#include <cfloat>
#include <memory>
#include <utility>

struct Hit {
  Point3 location;
  Vec3 normal;
  std::shared_ptr<Material> material;
  double t;
  bool valid;

  double u{}, v{};
  bool front{};

  Hit() : t(DBL_MAX), valid(false) {}
  Hit(const Point3 &location,
      const Vec3 &normal,
      std::shared_ptr<Material> material,
      const double t, const double u, const double v,
      const bool front)
      : location(location),
        normal(normal),
        material(std::move(material)),
        t(t),
        u(u),
        v(v),
        front(front),
        valid(true) {}

  bool operator<(const Hit &o) const { return t < o.t; }
};

#endif //CPPTRACE_LIB_HITTABLE_HIT_HPP_
