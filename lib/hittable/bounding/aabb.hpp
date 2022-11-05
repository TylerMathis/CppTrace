//
// Created by Tyler Hostler-Mathis on 10/18/22.
//

#ifndef CPPTRACE_LIB_HITTABLE_BOUNDING_AABB_HPP_
#define CPPTRACE_LIB_HITTABLE_BOUNDING_AABB_HPP_

#include "../../common/ray.hpp"
#include "../../common/vec3.hpp"

#include <climits>

struct AxisAlignedBoundingBox {
  Point3 a, b;

  AxisAlignedBoundingBox() = default;

  [[maybe_unused]] AxisAlignedBoundingBox(const Point3 &a, const Point3 &b) : a(
      a), b(b) {}

  // Pixar AABB hit optimization -- Andrew Kensler
  [[nodiscard]] bool hit(const Ray &ray,
                         const double minT,
                         const double maxT) const {
    for (int i = 0; i < 3; i++) {
      double invD = 1 / ray.direction[i];
      double t0 = (a[i] - ray.origin[i]) * invD;
      double t1 = (b[i] - ray.origin[i]) * invD;
      if (invD < 0)
        std::swap(t0, t1);
      double tMin = std::max((double) minT, t0);
      double tMax = std::min((double) maxT, t1);
      if (tMax <= tMin)
        return false;
    }
    return true;
  }
};

using AABB = AxisAlignedBoundingBox;

AABB surroundingBox(AABB &box0, AABB &box1) {
  Point3 a(std::min(box0.a.x, box1.a.x), std::min(box0.a.y, box1.a.y),
           std::min(box0.a.z, box1.a.z));

  Point3 b(std::max(box0.b.x, box1.b.x), std::max(box0.b.y, box1.b.y),
           std::max(box0.b.z, box1.b.z));

  return {a, b};
}

#endif //CPPTRACE_LIB_HITTABLE_BOUNDING_AABB_HPP_
