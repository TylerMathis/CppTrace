#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "../Ray.h"
#include "./Hit.h"
#include "Hittable.h"

#include <memory>
#include <vector>

template <typename T, int minT = 0, int maxT = INT_MAX>
struct _HittableList : public _Hittable<T, minT, maxT> {
  std::vector<std::shared_ptr<Hittable>> hittables;

  _HittableList() {}
  _HittableList(std::vector<std::shared_ptr<Hittable>> &hittables)
      : hittables(hittables) {}

  void pushHittable(std::shared_ptr<Hittable> hittable) {
    hittables.push_back(hittable);
  }
  void loadHittable(std::shared_ptr<Hittable> hittable) {
    hittables = {hittable};
  }
  void loadHittables(std::vector<std::shared_ptr<Hittable>> _hittables) {
    hittables = _hittables;
  }

  virtual bool hit(const Ray &ray, Hit &hit) const override {
    Hit out, closest;
    bool found = false;
    for (auto hittable : hittables)
      if (hittable->hit(ray, out) && out < closest) {
        closest = out;
        found = true;
      }

    if (found)
      hit = closest;

    return found;
  }

  virtual bool boundingBox(const double t0, const double t1,
                           AABB &out) const override {
    if (hittables.empty())
      return false;

    AABB box;
    bool first = true;

    for (auto &hittable : hittables) {
      if (!hittable->boundingBox(t0, t1, box))
        return false;
      out = first ? box : surroundingBox(box, out);
      first = false;
    }

    return true;
  }
};

using HittableList = _HittableList<double>;

#endif