//
// Created by Tyler Hostler-Mathis on 10/18/22.
//

#ifndef RAYTRACER_LIB_HITTABLE_BOUNDING_BVH_HPP_
#define RAYTRACER_LIB_HITTABLE_BOUNDING_BVH_HPP_

#include "../../common/common.hpp"
#include "../../common/ray.hpp"
#include "../hit.hpp"
#include "../hittable.hpp"
#include "../hittable_list.hpp"
#include "aabb.hpp"

#include <cassert>
#include <climits>
#include <memory>
#include <random>
#include <stdexcept>
#include <vector>
#include <iostream>

struct BVHNode : Hittable {
  std::vector<std::shared_ptr<Hittable>> children;
  AABB box;

  BVHNode() = default;

  explicit BVHNode(const HittableList &objects)
      : BVHNode(objects.hittables, 0, (int) objects.hittables.size() - 1) {}

  BVHNode(const std::vector<std::shared_ptr<Hittable>> &_objects,
          const int start, const int end) {
    if (_objects.empty()) return;
    auto objects = _objects;

    int axis = common::randomInt(0, 2);
    std::sort(objects.begin() + start,
              objects.begin() + end + 1,
              [&axis](const std::shared_ptr<Hittable> &a,
                      const std::shared_ptr<Hittable> &b) {
                return boxCompare(a, b, axis);
              });

    int span = end - start + 1;
    if (span <= 8) {
      children = {begin(objects) + start, begin(objects) + end + 1};
    } else {
      int mid = (start + end) >> 1;
      auto left = std::make_shared<BVHNode>(objects, start, mid);
      auto right = std::make_shared<BVHNode>(objects, mid + 1, end);
      children = {left, right};
    }

    box = children[0]->boundingBox();
    for (int i = 1; i < children.size(); i++) {
      AABB childBox = children[i]->boundingBox();
      box = surroundingBox(box, childBox);
    }
  }

  bool hit(const Ray &ray,
           Hit &hit,
           const double minT,
           const double maxT) const override {
    if (!box.hit(ray, minT, maxT))
      return false;

    bool foundHit = false;
    Hit childHit;
    for (const auto &child : children) {
      foundHit |= child->hit(ray, childHit, minT, maxT);
      hit = std::min(hit, childHit);
    }

    return foundHit;
  }

  [[nodiscard]] AABB boundingBox() const override { return box; }
};

using BVH = BVHNode;

#endif //RAYTRACER_LIB_HITTABLE_BOUNDING_BVH_HPP_
