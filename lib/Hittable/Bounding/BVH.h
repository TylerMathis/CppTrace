#ifndef BVH_H
#define BVH_H

#include "../../Common.h"
#include "../../Ray.h"
#include "../Hit.h"
#include "../Hittable.h"
#include "../HittableList.h"
#include "AABB.h"

#include <climits>
#include <memory>
#include <random>
#include <stdexcept>
#include <vector>

struct BVHNode : Hittable {
  std::shared_ptr<Hittable> left, right;
  AABB box;

  BVHNode() = default;

  explicit BVHNode(const HittableList &objects)
      : BVHNode(objects.hittables, 0, (int) objects.hittables.size() - 1) {}

  BVHNode(const std::vector<std::shared_ptr<Hittable>> &_objects,
          const int start, const int end) {
    auto objects = _objects;

    int axis = common::randomInt(0, 2);
    auto cmp = [&axis](const std::shared_ptr<Hittable> &a,
                       const std::shared_ptr<Hittable> &b) {
      return boxCompare(a, b, axis);
    };

    int span = end - start + 1;
    if (span == 1) {
      left = right = objects[start];
    } else if (span == 2) {
      if (cmp(objects[start], objects[start + 1])) {
        left = objects[start];
        right = objects[start + 1];
      } else {
        left = objects[start + 1];
        right = objects[start];
      }
    } else {
      std::sort(objects.begin() + start, objects.begin() + end + 1, cmp);

      int mid = (start + end) >> 1;
      left = std::make_shared<BVHNode>(objects, start, mid);
      right = std::make_shared<BVHNode>(objects, mid + 1, end);
    }

    AABB boxLeft = left->boundingBox(), boxRight = right->boundingBox();
    box = surroundingBox(boxLeft, boxRight);
  }

  bool hit(const Ray &ray,
           Hit &hit,
           const double minT,
           const double maxT) const override {
    if (!box.hit(ray, minT, maxT))
      return false;

    Hit l, r;
    bool hLeft = left->hit(ray, l, minT, maxT);
    bool hRight = right->hit(ray, r, minT, maxT);
    hit = std::min(l, r);

    return hLeft || hRight;
  }

  [[nodiscard]] AABB boundingBox() const override { return box; }
};

using BVH = BVHNode;

#endif