//
// Created by Tyler Hostler-Mathis on 11/30/22.
//

#ifndef CPPTRACE_LIB_ACCELERATORS_FAST_BVH_HPP_
#define CPPTRACE_LIB_ACCELERATORS_FAST_BVH_HPP_

#include "../hittable/hittable.hpp"
#include "../hittable/hit.hpp"
#include "../hittable/objects/triangle.hpp"
#include "../common/ray.hpp"
#include "../hittable/bounding/aabb.hpp"

#include <FastBVH.h>
#include <FastBVH/Traverser.h>

#include <type_traits>

class TriangleBoxConverter final {
 public:
  FastBVH::BBox<double> operator()(const std::shared_ptr<Triangle> &s) const noexcept {
    auto box = s->boundingBox();
    FastBVH::Vector3<double> min{
        box.a.x,
        box.a.y,
        box.a.z,
    };
    FastBVH::Vector3<double> max{
        box.b.x,
        box.b.y,
        box.b.z,
    };
    return {min, max};
  }
};

class TriangleIntersector final {
 public:
  FastBVH::Intersection<double, std::shared_ptr<Triangle>> operator()(const std::shared_ptr<Triangle> &triangle,
                                                                      const FastBVH::Ray<double> &fray) const noexcept {
    Point3 origin(fray.o.x, fray.o.y, fray.o.z);
    Vec3 direction(fray.d.x, fray.d.y, fray.d.z);
    Ray ray(origin, direction);

    Hit hit = triangle->hit(ray, 0.001, DBL_MAX);
    if (!hit.valid) {
      return {};
    }

    FastBVH::Vector3<double> normal{
        hit.normal.x,
        hit.normal.y,
        hit.normal.z
    };

    return {
        hit.t,
        &triangle,
        normal,
    };
  }
};

struct FastBVHImpl : public Hittable {
  FastBVH::DefaultBuilder<double> builder;
  TriangleBoxConverter triangleConverter;
  TriangleIntersector intersector;
  FastBVH::BVH<double, std::shared_ptr<Triangle>> bvh;
  FastBVH::Traverser<double, std::shared_ptr<Triangle>, TriangleIntersector> traverser;
  AABB box;

  FastBVHImpl(std::vector<std::shared_ptr<Triangle>> &tris, std::shared_ptr<Material> &material) : bvh(builder(tris,
                                                                                                               triangleConverter)),
                                                                                                   traverser(FastBVH::Traverser(
                                                                                                       bvh,
                                                                                                       intersector)) {
    this->material = material;

    if (tris.empty())
      return;

    box = tris[0]->boundingBox();
    for (int i = 1; i < tris.size(); i++)
      box = surroundingBox(box, tris[i]->boundingBox());
  }

  [[nodiscard]] Hit hit(const Ray &ray,
                        double minT,
                        double maxT) const override {
    FastBVH::Vector3<double> origin{};
    origin.x = ray.origin.x;
    origin.y = ray.origin.y;
    origin.z = ray.origin.z;
    FastBVH::Vector3<double> direction{};
    direction.x = ray.direction.x;
    direction.y = ray.direction.y;
    direction.z = ray.direction.z;
    FastBVH::Ray fray(origin, direction);

    auto isect = traverser.traverse(fray);
    if (isect) {
      return isect.object->get()->hit(ray, minT, maxT);
    } else return {};
  }

  [[nodiscard]] AABB boundingBox() const override {
    return box;
  }
};

#endif //CPPTRACE_LIB_ACCELERATORS_FAST_BVH_HPP_
