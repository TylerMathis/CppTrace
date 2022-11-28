//
// Created by Tyler Hostler-Mathis on 10/18/22.
//

#ifndef CPPTRACE_LIB_HITTABLE_BOUNDING_AABB_HPP_
#define CPPTRACE_LIB_HITTABLE_BOUNDING_AABB_HPP_

#include "../../common/ray.hpp"
#include "../../common/vec3.hpp"

#include <algorithm>
#include <cassert>
#include <climits>

struct AxisAlignedBoundingBox {
  Point3 a, b;

  AxisAlignedBoundingBox() = default;

  [[maybe_unused]] AxisAlignedBoundingBox(const Point3 &a, const Point3 &b)
      : a(a), b(b) {}

  // Pixar AABB hit optimization -- Andrew Kensler
  [[nodiscard]] bool hit(const Ray &ray, const double minT,
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

  std::pair<int, int> MaximumExtent() const {
    std::vector<std::pair<double, int>> bounds = {{std::abs(a.x - b.x), 0},
                                                  {std::abs(a.y - b.y), 1},
                                                  {std::abs(a.z - b.z), 2}};
    std::sort(begin(bounds), end(bounds));
    return {bounds[0].second, bounds[0].first};
  }

  // returns a list of rays representing the AABB where the domain of 't' for
  // each ray is [0, 1]
  std::vector<Ray> getRays() const {
    std::vector<Ray> rays;
    auto constructPoint = [&](int mask) {
      Point3 p;
      p.x = (mask & 1) ? b.x : a.x;
      p.y = (mask & 2) ? b.y : a.y;
      p.z = (mask & 4) ? b.z : a.z;
      return p;
    };
    for (int mask = 0; mask < 8; mask++) {
      auto p = constructPoint(mask);
      for (int supermask = mask; supermask < 8;
           supermask = (supermask + 1) | mask) {
        if (__builtin_popcount(mask ^ supermask) == 1) {
          auto q = constructPoint(supermask);
          rays.emplace_back(p, q - p);
        }
      }
    }
    assert(rays.size() == 12);
    return rays;
  }
};

using AABB = AxisAlignedBoundingBox;

AABB surroundingBox(const AABB &box0, const AABB &box1) {
  Point3 a(std::min(box0.a.x, box1.a.x), std::min(box0.a.y, box1.a.y),
           std::min(box0.a.z, box1.a.z));

  Point3 b(std::max(box0.b.x, box1.b.x), std::max(box0.b.y, box1.b.y),
           std::max(box0.b.z, box1.b.z));

  return {a, b};
}

std::pair<AABB, AABB> splitBoxOnX(const AABB &box, int dist) {
  assert(box.a.x + dist <= box.b.x);
  auto box0 = box;
  auto box1 = box;
  box0.b.x = box1.a.x = box.a.x + dist;
  return {box0, box1};
}

std::pair<AABB, AABB> splitBoxOnY(const AABB &box, int dist) {
  assert(box.a.y + dist <= box.b.y);
  auto box0 = box;
  auto box1 = box;
  box0.b.y = box1.a.y = box.a.y + dist;
  return {box0, box1};
}

std::pair<AABB, AABB> splitBoxOnZ(const AABB &box, int dist) {
  assert(box.a.z + dist <= box.b.z);
  auto box0 = box;
  auto box1 = box;
  box0.b.z = box1.a.z = box.a.z + dist;
  return {box0, box1};
}

std::pair<AABB, AABB> splitBox(const AABB &box, int axis, int dist) {
  switch (axis) {
    case 0:return splitBoxOnX(box, dist);
    case 1:return splitBoxOnY(box, dist);
    default:return splitBoxOnZ(box, dist);
  }
}

bool overlap(std::pair<int, int> a, std::pair<int, int> b) {
  return a.first < b.second && b.first < a.second;
}

bool boxOverlap(const AABB &box0, const AABB &box1) {
  return overlap({box0.a.x, box0.b.x}, {box1.a.x, box1.b.x}) &&
      overlap({box0.a.y, box0.b.y}, {box1.a.y, box1.b.y}) &&
      overlap({box0.a.z, box0.b.z}, {box1.a.z, box1.b.z});
}

#endif // CPPTRACE_LIB_HITTABLE_BOUNDING_AABB_HPP_
