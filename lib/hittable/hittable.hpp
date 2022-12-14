//
// Created by Tyler Hostler-Mathis on 10/18/22.
//

#ifndef CPPTRACE_LIB_HITTABLE_HITTABLE_HPP_
#define CPPTRACE_LIB_HITTABLE_HITTABLE_HPP_

#include "../common/ray.hpp"
#include "bounding/aabb.hpp"
#include "materials/material.hpp"
#include "hit.hpp"

#include <memory>
#include <climits>
#include <stdexcept>
#include <vector>

struct Triangle;

struct Hittable {
  std::shared_ptr<Material> material;

  [[nodiscard]] virtual Hit hit(const Ray &ray,
                                double minT,
                                double maxT) const = 0;

  [[nodiscard]] virtual AABB boundingBox() const = 0;

  [[nodiscard]] virtual std::vector<std::shared_ptr<Hittable>> getHittables() const { return {}; }
  [[nodiscard]] virtual std::vector<std::shared_ptr<Triangle>> getTriangles() const { return {}; }
};

inline bool boxCompare(const std::shared_ptr<Hittable> &a,
                       const std::shared_ptr<Hittable> &b, int axis) {
  auto boxA = a->boundingBox(), boxB = b->boundingBox();
  return boxA.a[axis] < boxB.a[axis];
}

#endif //CPPTRACE_LIB_HITTABLE_HITTABLE_HPP_
