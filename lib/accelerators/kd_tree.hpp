//
// Created by Cameron Custer on 11/8/22.
//

#ifndef CPPTRACE_LIB_HITTABLE_KD_TREE_HPP_
#define CPPTRACE_LIB_HITTABLE_KD_TREE_HPP_

#include "../common/common.hpp"
#include "../common/ray.hpp"
#include "../hittable/bounding/aabb.hpp"
#include "../hittable/hit.hpp"
#include "../hittable/hittable.hpp"
#include "../hittable/hittable_list.hpp"

struct KDTree : Hittable {
  std::vector<std::shared_ptr<Hittable>> children;
  AABB box;

  KDTree() = default;

  bool hit(const Ray &ray, Hit &hit, const double minT,
           const double maxT) const override;

  [[nodiscard]] AABB boundingBox() const override { return box; }
};

#endif // CPPTRACE_LIB_HITTABLE_KD_TREE_HPP_
