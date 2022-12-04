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

#include <climits>
#include <memory>
#include <random>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <algorithm>
#include <tuple>

enum AXIS {
  X,
  Y,
  Z
};

struct BVHNode : public Hittable {
  std::vector<std::shared_ptr<Hittable>> children;
  AABB box;

  BVHNode() = default;

  explicit BVHNode(HittableList &objects)
      : BVHNode(objects.hittables, 0, (int) objects.hittables.size() - 1) {}

  static void sortOnAxis(std::vector<std::shared_ptr<Hittable>> &objects, AXIS axis, int start, int end) {
    std::sort(objects.begin() + start,
              objects.begin() + end + 1,
              [&axis](const std::shared_ptr<Hittable> &a,
                      const std::shared_ptr<Hittable> &b) {
                auto aCenter = a->getCenter();
                auto bCenter = b->getCenter();
                return aCenter[axis] < bCenter[axis];
              });
  }

  BVHNode(std::vector<std::shared_ptr<Hittable>> &objects,
          const int start, const int end) {
    if (objects.empty()) return;

    int span = end - start + 1;
    if (span <= 8) {
      children = std::vector<std::shared_ptr<Hittable>>(begin(objects) + start, begin(objects) + end + 1);
    } else {
      // [surface area, split, axis]
      std::tuple<double, int, AXIS> bestSplit = {DBL_MAX, start, X};
      for (int axisInt = 0; axisInt < 3; axisInt++) {
        auto axis = static_cast<const AXIS>(axisInt);
        sortOnAxis(objects, axis, start, end);

        AABB LSA[span];
        LSA[0] = AABB(objects[start]->boundingBox());
        for (int l = 1; l < span; l++)
          LSA[l] = surroundingBox(LSA[l - 1], objects[start + l]->boundingBox());

        AABB RSA[span];
        RSA[span - 1] = AABB(objects[end]->boundingBox());
        for (int r = 1; r < span; r++)
          RSA[span - 1 - r] = surroundingBox(RSA[span - r], objects[end - r]->boundingBox());

        for (int i = 0; i < span - 1; i++) {
          double cost = surfaceArea(LSA[i]) * i + surfaceArea(RSA[i]) * (span - i);
          int split = start + i;
          bestSplit = std::min(bestSplit, std::make_tuple(cost, split, axis));
        }
      }

      sortOnAxis(objects, get<2>(bestSplit), start, end);
      auto left = std::make_shared<BVHNode>(objects, start, get<1>(bestSplit));
      auto right = std::make_shared<BVHNode>(objects, get<1>(bestSplit) + 1, end);
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
