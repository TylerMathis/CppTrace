#ifndef BVH_H
#define BVH_H

#include "../../Common.h"
#include "../../Ray.h"
#include "../Hit.h"
#include "../Hittable.h"
#include "../HittableList.h"
#include "AABB.h"

#include "limits.h"

#include <memory>
#include <random>
#include <stdexcept>
#include <vector>

template <typename T, int minT = 0, int maxT = INT_MAX>
struct _BVHNode : _Hittable<T, minT, maxT> {
  std::shared_ptr<Hittable> left, right;
  AABB box;

  _BVHNode() {}
  _BVHNode(const HittableList &objects)
      : _BVHNode(objects.hittables, 0, objects.hittables.size() - 1) {}
  _BVHNode(const std::vector<std::shared_ptr<Hittable>> &_objects,
           const size_t start, const size_t end) {
    auto objects = _objects;

    int axis = common::randomInt(0, 2);
    auto cmp = [&axis](const std::shared_ptr<Hittable> a,
                       const std::shared_ptr<Hittable> b) {
      return boxCompare(a, b, axis);
    };

    size_t span = end - start + 1;
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
      left = std::make_shared<_BVHNode>(objects, start, mid);
      right = std::make_shared<_BVHNode>(objects, mid + 1, end);
    }

    AABB boxLeft = left->boundingBox(), boxRight = right->boundingBox();
    box = surroundingBox(boxLeft, boxRight);
  }

  virtual bool hit(const Ray &ray, Hit &hit) const override {
    if (!box.hit(ray))
      return false;

    Hit l, r;
    bool hLeft = left->hit(ray, l);
    bool hRight = right->hit(ray, r);
    hit = std::min(l, r);

    return hLeft || hRight;
  }

  virtual AABB boundingBox() const override { return box; }
};

using BVH = _BVHNode<double>;

#endif