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
  Z,
};

enum SPLIT_METHOD {
  MIDDLE,
  SWEEP_SAH,
};

struct BVHNode : public Hittable {
  std::vector<std::shared_ptr<Hittable>> children;
  AABB box;

  BVHNode() = default;

  explicit BVHNode(HittableList &objects, SPLIT_METHOD splitMethod = SWEEP_SAH)
      : BVHNode(objects.hittables, 0, (int) objects.hittables.size() - 1, splitMethod) {}

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
          const int start, const int end, SPLIT_METHOD splitMethod) {
    if (objects.empty()) return;

    int span = end - start + 1;
    if (span <= 8) {
      children = std::vector<std::shared_ptr<Hittable>>(begin(objects) + start, begin(objects) + end + 1);
    } else {
      // [surface area, split, axis]
      int splitLoc;
      if (splitMethod == SWEEP_SAH) {
        std::tuple<double, int, AXIS> bestSplit = {DBL_MAX, start, X};
        for (int axisInt = 0; axisInt < 3; axisInt++) {
          auto axis = static_cast<const AXIS>(axisInt);
          sortOnAxis(objects, axis, start, end);

          std::vector<AABB> LSA;
          LSA.emplace_back(objects[start]->boundingBox());
          for (int l = 1; l < span; l++)
            LSA.push_back(surroundingBox(LSA[l - 1], objects[start + l]->boundingBox()));

          std::vector<AABB> RSA;
          RSA.emplace_back(objects[end]->boundingBox());
          for (int r = 1; r < span; r++)
            RSA.push_back(surroundingBox(RSA[r - 1], objects[end - r]->boundingBox()));
          std::reverse(std::begin(RSA), std::end(RSA));

          for (int i = 0; i < span - 1; i++) {
            double cost = surfaceArea(LSA[i]) * i + surfaceArea(RSA[i]) * (span - i);
            int split = start + i;
            bestSplit = std::min(bestSplit, std::make_tuple(cost, split, axis));
          }
        }

        sortOnAxis(objects, get<2>(bestSplit), start, end);
        splitLoc = get<1>(bestSplit);
      } else {
        auto axis = static_cast<const AXIS>(common::randomInt(0, 2));
        sortOnAxis(objects, axis, start, end);
        splitLoc = (start + end) >> 1;
      }

      auto left = std::make_shared<BVHNode>(objects, start, splitLoc, splitMethod);
      auto right = std::make_shared<BVHNode>(objects, splitLoc + 1, end, splitMethod);
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
