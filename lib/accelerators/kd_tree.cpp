//
// Created by Cameron Custer on 11/25/22.
//

#include "kd_tree.hpp"
#include "../common/ray.hpp"
#include "../hittable/bounding/aabb.hpp"
#include "../hittable/hit.hpp"
#include "../hittable/hittable.hpp"
#include "../hittable/hittable_list.hpp"

#include <memory>
#include <vector>

KDTree::KDTree(const HittableList &hittableList)
    : KDTree(hittableList.hittables) {}

KDTree::KDTree(const std::vector<std::shared_ptr<Hittable>> &hittables)
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

int KDTree::buildTree(const std::vector<int> indicies, AABB box, int depth) {
  if (depth > maxDepth)
    return -1;

  tree.emplace_back(indicies, box);

  auto [axis, dist] = box.MaximumExtent();
  auto [leftBox, rightBox] = splitBox(box, axis, dist / 2);

  auto leftBoxRays = leftBox.getRays();
  auto rightBoxRays = rightBox.getRays();

  auto hasIntersection = [](const auto &rays,
                            const std::shared_ptr<Hittable> &obj) -> bool {
    for (const auto &ray : rays)
      if (obj->hit(ray, 0, 1).valid)
        return true;
    return false;
  };

  std::vector<int> leftIndicies, rightIndicies;
  for (int i : indicies) {
    if (hasIntersection(leftBoxRays, objects[i]))
      leftIndicies.push_back(i);
    if (hasIntersection(rightBoxRays, objects[i]))
      rightIndicies.push_back(i);
  }

  int vert = tree.size();

  tree[vert].leftChildIndex = buildTree(leftIndicies, leftBox, depth + 1);
  tree[vert].rightChildIndex = buildTree(rightIndicies, rightBox, depth + 1);

  return vert;
}

Hit KDTree::traverseHit(int vert, const Ray &ray, const double minT,
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

Hit KDTree::hit(const Ray &ray, const double minT, const double maxT) const {
  return traverseHit(0, ray, minT, maxT);
}