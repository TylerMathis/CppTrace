#ifndef HITTABLE_H
#define HITTABLE_H

#include "../Ray.h"
#include "./Bounding/AABB.h"
#include "./Material/Material.h"
#include "Hit.h"

#include "limits.h"

#include <memory>
#include <stdexcept>

template <typename T, int minT = 0, int maxT = INT_MAX> struct _Hittable {
  std::shared_ptr<Material> material;

  virtual bool hit(const Ray &ray, Hit &hit) const = 0;
  virtual AABB boundingBox() const = 0;

  bool validT(const double t) const {
    return t >= (minT + 0.0001) && t <= maxT;
  }
};

inline bool boxCompare(const std::shared_ptr<_Hittable<double>> a,
                       const std::shared_ptr<_Hittable<double>> b, int axis) {
  auto boxA = a->boundingBox(), boxB = b->boundingBox();
  return boxA.a[axis] < boxB.a[axis];
}

using Hittable = _Hittable<double>;

#endif
