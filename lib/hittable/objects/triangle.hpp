//
// Created by Tyler Hostler-Mathis on 10/18/22.
//

#ifndef CPPTRACE_LIB_HITTABLE_TRIANGLE_HPP_
#define CPPTRACE_LIB_HITTABLE_TRIANGLE_HPP_

#include "../../common/ray.hpp"
#include "../../common/vec3.hpp"
#include "../bounding/aabb.hpp"
#include "../materials/material.hpp"
#include "../hit.hpp"
#include "../hittable.hpp"

#include <climits>
#include <memory>
#include <vector>

// CCW Triangle
struct Triangle : public Hittable {
  const double EPS = 1e-6;

  Point3 p0;
  Vec3 e1, e2;
  Normal normal;

  Triangle() = default;
  Triangle(const Point3 &p0,
           const Point3 &p1,
           const Point3 &p2,
           const std::shared_ptr<Material> &material)
      : p0(p0), e1(p0 - p1), e2(p2 - p0),
        normal(e1.cross(e2)) {
    this->material = material;
  }

  [[nodiscard]] Point3 p1() const { return p0 - e1; }
  [[nodiscard]] Point3 p2() const { return p0 + e2; }

  [[nodiscard]] Hit hit(const Ray &ray,
                        const double minT,
                        const double maxT) const override {
    auto c = p0 - ray.origin;
    auto r = ray.direction.cross(c);
    auto invDet = 1.0 / normal.dot(ray.direction);

    auto u = r.dot(e2) * invDet;
    auto v = r.dot(e1) * invDet;
    auto w = 1.0 - u - v;

    if (u >= EPS && v >= EPS && w >= EPS) {
      auto t = normal.dot(c) * invDet;
      if (t >= minT && t <= maxT) {
        return {ray.at(t), normal, this->material, t, u, v, true};
      }
    }

    return {};
  }

  [[nodiscard]] AABB boundingBox() const override {
    AABB box(p0);
    box = surroundingBox(box, AABB(p1()));
    box = surroundingBox(box, AABB(p2()));
    return box;
  }

  [[nodiscard]] Point3 getCenter() const override {
    return p0 + (e1 / 3.0) - (e2 / 3.0);
  }
};

#endif //CPPTRACE_LIB_HITTABLE_TRIANGLE_HPP_
