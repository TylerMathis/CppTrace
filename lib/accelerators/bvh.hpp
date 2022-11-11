//
// Created by Tyler Hostler-Mathis on 10/18/22.
//

#ifndef CPPTRACE_LIB_HITTABLE_BOUNDING_BVH_HPP_
#define CPPTRACE_LIB_HITTABLE_BOUNDING_BVH_HPP_

#include "../common/common.hpp"
#include "../common/ray.hpp"
#include "../hittable/hit.hpp"
#include "../hittable/hittable.hpp"
#include "../hittable/hittable_list.hpp"
#include "../hittable/bounding/aabb.hpp"

#include <cassert>
#include <climits>
#include <memory>
#include <random>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <algorithm>

struct BVHNode : Hittable {
  std::vector<std::shared_ptr<Hittable>> children;
  AABB box;

  BVHNode() = default;

  explicit BVHNode(HittableList &objects)
      : BVHNode(objects.hittables, 0, (int) objects.hittables.size() - 1) {}

  BVHNode(std::vector<std::shared_ptr<Hittable>> &objects,
          const int start, const int end) {
    if (objects.empty()) return;

    int axis = common::randomInt(0, 2);
    std::sort(objects.begin() + start,
              objects.begin() + end + 1,
              [&axis](const std::shared_ptr<Hittable> &a,
                      const std::shared_ptr<Hittable> &b) {
                return boxCompare(a, b, axis);
              });

    int span = end - start + 1;
    if (span <= 8) {
      assert(start >= 0 && start < size(objects));
      assert(end >= 0 && end < size(objects));
      children = std::vector<std::shared_ptr<Hittable>>(begin(objects) + start, begin(objects) + end + 1);
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

  [[nodiscard]] Hit hit(const Ray &ray,
                        const double minT,
                        const double maxT) const override {
    if (!box.hit(ray, minT, maxT))
      return {};

    Hit minHit;
    for (const auto &child : children) {
      Hit childHit = child->hit(ray, minT, maxT);
      if (childHit.valid && childHit < minHit) {
        minHit = childHit;
      }
    }

    return minHit;
  }

  [[nodiscard]] AABB boundingBox() const override { return box; }
};

using BVH = BVHNode;

#endif //CPPTRACE_LIB_HITTABLE_BOUNDING_BVH_HPP_
