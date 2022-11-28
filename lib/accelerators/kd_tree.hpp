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
#include <utility>
#include <vector>

struct KDTree : public Hittable {
  std::vector<std::shared_ptr<Hittable>> objects;
  struct Node {
    AABB box;
    std::vector<int> objectIndicies;
    int leftChildIndex = -1, rightChildIndex = -1;
    Node(std::vector<int> aObjectIndexList, const AABB &aBox)
        : box(aBox), objectIndicies(std::move(aObjectIndexList)) {}
    [[nodiscard]] constexpr bool isLeaf() const {
      return leftChildIndex == -1 && rightChildIndex == -1;
    }
  };
  std::vector<Node> tree;
  int maxDepth;

  KDTree() = default;

  explicit KDTree(const HittableList &hittableList) : KDTree(hittableList.hittables) {}

  explicit KDTree(const std::vector<std::shared_ptr<Hittable>> &hittables)
      : objects(hittables), maxDepth(8 + 1.3 * std::log(hittables.size())) {
    std::vector<int> indicies(hittables.size());
    std::iota(indicies.begin(), indicies.end(), 0);
    AABB box = hittables[0]->boundingBox();
    for (int i = 1; i < hittables.size(); i++) {
      AABB childBox = hittables[i]->boundingBox();
      box = surroundingBox(box, childBox);
    }
    buildTree(indicies, box, 0);
  }

  int buildTree(const std::vector<int> indicies, AABB box, int depth) {
    if (indicies.empty() || depth > maxDepth)
      return -1;

    tree.emplace_back(indicies, box);

    auto [axis, dist] = box.MaximumExtent();
    auto [leftBox, rightBox] = splitBox(box, axis, dist / 2);

    auto leftBoxRays = leftBox.getRays();
    auto rightBoxRays = rightBox.getRays();

    auto hasIntersection = [](const auto &rays,
                              const std::shared_ptr<Hittable> &obj) -> bool {
      return std::any_of(begin(rays), end(rays), [&obj](const auto &ray) {
        return obj->hit(ray, 0, 1).valid;
      });
    };

    std::vector<int> leftIndicies, rightIndicies;
    for (int i : indicies) {
      if (hasIntersection(leftBoxRays, objects[i]))
        leftIndicies.push_back(i);
      if (hasIntersection(rightBoxRays, objects[i]))
        rightIndicies.push_back(i);
    }

    int vert = (int) tree.size() - 1;

    tree[vert].leftChildIndex = buildTree(leftIndicies, leftBox, depth + 1);
    tree[vert].rightChildIndex = buildTree(rightIndicies, rightBox, depth + 1);

    return vert;
  }

  [[nodiscard]] Hit traverseHit(int vert, const Ray &ray, const double minT,
                  const double maxT) const {
    if (vert == -1 || !tree[vert].box.hit(ray, minT, maxT))
      return {};

    if (tree[vert].isLeaf()) {
      Hit minHit;
      for (int i : tree[vert].objectIndicies) {
        auto objHit = objects[i]->hit(ray, minT, maxT);
        if (objHit.valid && objHit < minHit) {
          minHit = objHit;
        }
      }
      return minHit;
    }

    Hit minHit;
    for (auto childIndex :
        {tree[vert].leftChildIndex, tree[vert].rightChildIndex}) {
      Hit childHit = traverseHit(childIndex, ray, minT, maxT);
      if (childHit.valid && childHit < minHit) {
        minHit = childHit;
      }
    }

    return minHit;
  }

  [[nodiscard]] Hit hit(const Ray &ray, const double minT, const double maxT) const override {
    return traverseHit(0, ray, minT, maxT);
  }

  [[nodiscard]] AABB boundingBox() const override { return tree[0].box; }
};

#endif // CPPTRACE_LIB_HITTABLE_KD_TREE_HPP_
