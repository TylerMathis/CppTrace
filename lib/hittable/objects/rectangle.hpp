//
// Created by Tyler Hostler-Mathis on 10/23/22.
//

#ifndef CPPTRACE_LIB_HITTABLE_OBJECTS_RECTANGLE_HPP_
#define CPPTRACE_LIB_HITTABLE_OBJECTS_RECTANGLE_HPP_

#include "../../common/ray.hpp"
#include "../../common/vec3.hpp"
#include "../bounding/aabb.hpp"
#include "../objects/triangle.hpp"
#include "../hittable.hpp"
#include "../hit.hpp"

struct Rectangle : Hittable {
  Triangle a, b;

  Rectangle() = default;
  // Reflects point b of a triangle to make the rectangle
  explicit Rectangle(const Triangle &a)
      : a(a), b(a.a, a.b + (a.a - a.b) + (a.c - a.b), a.c, a.material) {}

  bool hit(const Ray &ray, Hit &hit, double minT, double maxT) const override {
    bool didHit = false;

    Hit h1, h2;
    didHit |= a.hit(ray, h1, minT, maxT);
    didHit |= b.hit(ray, h2, minT, maxT);

    hit = std::min(h1, h2);
    return didHit;
  }

  [[nodiscard]] AABB boundingBox() const override {
    auto aBox = a.boundingBox(), bBox = b.boundingBox();
    return surroundingBox(aBox, bBox);
  }
};

#endif //CPPTRACE_LIB_HITTABLE_OBJECTS_RECTANGLE_HPP_
