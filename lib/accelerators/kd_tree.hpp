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

#include <memory>
#include <vector>

struct KDTree : public Hittable {
  std::vector<std::shared_ptr<Hittable>> objects;
  struct Node {
    AABB box;
    std::vector<int> objectIndicies;
    int leftChildIndex, rightChildIndex;
  };
  std::vector<Node> tree;

  KDTree() = default;

  KDTree(const HittableList &);

  KDTree(const std::vector<std::shared_ptr<Hittable>> &);

  int buildTree(const std::vector<int>, AABB);

  Hit hit(const Ray &, const double, const double) const override;

  [[nodiscard]] AABB boundingBox() const override { return tree[0].box; }
};

#endif // CPPTRACE_LIB_HITTABLE_KD_TREE_HPP_
