//
// Created by Tyler Hostler-Mathis on 10/18/22.
//

#ifndef CPPTRACE_LIB_HITTABLE_HITTABLE_LIST_HPP_
#define CPPTRACE_LIB_HITTABLE_HITTABLE_LIST_HPP_

#include "../common/ray.hpp"
#include "hit.hpp"
#include "hittable.hpp"

#include <climits>
#include <memory>
#include <utility>
#include <vector>

struct HittableList : public Hittable {
  std::vector<std::shared_ptr<Hittable>> hittables;

  HittableList() = default;
  explicit HittableList(std::vector<std::shared_ptr<Hittable>> hittables)
      : hittables(std::move(hittables)) {}

  void pushHittable(const std::shared_ptr<Hittable> &hittable) {
    hittables.push_back(hittable);
  }
  void loadHittable(const std::shared_ptr<Hittable> &hittable) {
    hittables = {hittable};
  }
  void loadHittables(const std::vector<std::shared_ptr<Hittable>> &_hittables) {
    hittables = _hittables;
  }
  void clear() {
    hittables.clear();
  }

  [[nodiscard]] Hit hit(const Ray &ray, const double minT, const double maxT) const override {
    Hit hit, closest;
    bool found = false;
    for (const auto &hittable : hittables)
      hit = hittable->hit(ray, minT, maxT);
      if (hit.valid && hit < closest) {
        closest = hit;
        found = true;
      }

    return closest;
  }

  [[nodiscard]] AABB boundingBox() const override {
    bool first = true;
    AABB out;
    for (auto &hittable : hittables) {
      AABB box = hittable->boundingBox();
      out = first ? box : surroundingBox(box, out);
      first = false;
    }
    return out;
  }
};

#endif //CPPTRACE_LIB_HITTABLE_HITTABLE_LIST_HPP_
