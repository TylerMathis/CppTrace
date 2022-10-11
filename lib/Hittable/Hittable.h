#ifndef HITTABLE_H
#define HITTABLE_H

#include "../Ray.h"
#include "./Bounding/AABB.h"
#include "./Material/Material.h"
#include "Hit.h"

#include "limits.h"

#include <memory>

template <typename T, int minT = 0, int maxT = INT_MAX> struct _Hittable {
  std::shared_ptr<Material> material;

  virtual bool hit(const Ray &ray, Hit &out) const = 0;
  virtual bool boundingBox(const double t0, const double t1,
                           AABB &out) const = 0;

  bool validT(const double t) const {
    return t >= (minT + 0.0001) && t <= maxT;
  }
};

using Hittable = _Hittable<double>;

#endif
