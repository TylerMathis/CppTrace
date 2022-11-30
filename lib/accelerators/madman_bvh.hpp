//
// Created by tyler on 11/11/2022.
//

#ifndef CPPTRACE_LIB_ACCELERATORS_MADMAN_BVH_HPP_
#define CPPTRACE_LIB_ACCELERATORS_MADMAN_BVH_HPP_

#include "../common/common.hpp"
#include "../common/ray.hpp"
#include "../hittable/hit.hpp"
#include "../hittable/hittable.hpp"
#include "../hittable/hittable_list.hpp"
#include "../hittable/bounding/aabb.hpp"
#include "../hittable/objects/triangle.hpp"

#include "../extern/bvh/include/bvh/bvh.hpp"
#include "../extern/bvh/include/bvh/vector.hpp"
#include "../extern/bvh/include/bvh/triangle.hpp"
#include "../extern/bvh/include/bvh/sphere.hpp"
#include "../extern/bvh/include/bvh/ray.hpp"
#include "../extern/bvh/include/bvh/sweep_sah_builder.hpp"
#include "../extern/bvh/include/bvh/single_ray_traverser.hpp"
#include "../extern/bvh/include/bvh/primitive_intersectors.hpp"

using Scalar = double;
using Vector3 = bvh::Vector3<Scalar>;
using BTriangle = bvh::Triangle<Scalar>;
using Sphere = bvh::Sphere<Scalar>;
using BRay = bvh::Ray<Scalar>;
using Bvh = bvh::Bvh<Scalar>;

struct MadmanBVH : public Hittable {
  std::vector<BTriangle> triangles;
  Bvh bvh;
  bvh::SingleRayTraverser<Bvh> *traverser;
  bvh::ClosestPrimitiveIntersector<Bvh, BTriangle> *primitive_intersector;
  bvh::BoundingBox<double> global_bbox;

  ~MadmanBVH() {
    delete traverser;
    delete primitive_intersector;
  }
  MadmanBVH(std::vector<std::shared_ptr<Triangle>> &tris, std::shared_ptr<Material> &material) {
    this->material = material;

    for (const auto &tri_ref : tris) {
      auto tri = *tri_ref;
      triangles.emplace_back(
          Vector3(tri.p0.x, tri.p0.y, tri.p0.z),
          Vector3(tri.p1().x, tri.p1().y, tri.p1().z),
          Vector3(tri.p2().x, tri.p2().y, tri.p2().z)
      );
    }

    auto [bboxes, centers] = bvh::compute_bounding_boxes_and_centers(triangles.data(), triangles.size());
    global_bbox = bvh::compute_bounding_boxes_union(bboxes.get(), triangles.size());

    // Create an acceleration data structure on the primitives
    bvh::SweepSahBuilder<Bvh> builder(bvh);
    builder.build(global_bbox, bboxes.get(), centers.get(), triangles.size());
    primitive_intersector = new bvh::ClosestPrimitiveIntersector<Bvh, BTriangle>(bvh, triangles.data());
    traverser = new bvh::SingleRayTraverser<Bvh>(bvh);
  }

  [[nodiscard]] Hit hit(const Ray &ray, const double minT, const double maxT) const
  override {
    Vector3 o(ray.origin.x, ray.origin.y, ray.origin.z);
    Vector3 d(ray.direction.x, ray.direction.y, ray.direction.z);
    d = normalize(d);
    BRay test(o, d, minT, maxT);

    if (auto hit = traverser->traverse(test, *primitive_intersector)) {
      auto btriangle = triangles[hit->primitive_index];
      auto intersection = hit->intersection;
      auto bhitLoc = btriangle.p0 + btriangle.e1 * intersection.u + btriangle.e2 * intersection.v;

      auto hitLoc = Vec3(bhitLoc[0], bhitLoc[1], bhitLoc[2]);
      auto triangle = Triangle(
          Point3(btriangle.p0.values[0], btriangle.p0.values[1], btriangle.p0.values[2]),
          Point3(btriangle.p1().values[0], btriangle.p1().values[1], btriangle.p1().values[2]),
          Point3(btriangle.p2().values[0], btriangle.p2().values[1], btriangle.p2().values[2]),
          this->material
      );

      return {hitLoc, triangle.normal, this->material, intersection.t, intersection.u, intersection.v, true};
    }
    return {};
  }

  [[nodiscard]] AABB boundingBox() const override {
    AABB ret;
    ret.a = Point3(global_bbox.min.values[0], global_bbox.min.values[1], global_bbox.min.values[2]);
    ret.b = Point3(global_bbox.max.values[0], global_bbox.max.values[1], global_bbox.max.values[2]);
    return ret;
  }
};

#endif //CPPTRACE_LIB_ACCELERATORS_MADMAN_BVH_HPP_
