#ifndef HITTABLE_H
#define HITTABLE_H

#include "../Ray.h"
#include "./Bounding/AABB.h"
#include "./Material/Material.h"
#include "Hit.h"

#include <memory>
#include <climits>
#include <stdexcept>

struct Hittable {
  std::shared_ptr<Material> material;

  virtual bool hit(const Ray &ray, Hit &hit, double minT, double maxT) const = 0;

  [[nodiscard]] virtual AABB boundingBox() const = 0;
};

inline bool boxCompare(const std::shared_ptr<Hittable> &a,
                       const std::shared_ptr<Hittable> &b, int axis) {
  auto boxA = a->boundingBox(), boxB = b->boundingBox();
  return boxA.a[axis] < boxB.a[axis];
}

#endif
