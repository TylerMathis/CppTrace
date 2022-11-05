//
// Created by Tyler Hostler-Mathis on 10/18/22.
//

#ifndef CPPTRACE_LIB_HITTABLE_OBJ_HPP_
#define CPPTRACE_LIB_HITTABLE_OBJ_HPP_

#include "../../common/vec3.hpp"
#include "../../accelerators/bvh.hpp"
#include "../materials/lambertian.hpp"
#include "../hittable.hpp"
#include "../hittable_list.hpp"
#include "triangle.hpp"

#include "../../extern/obj_loader.hpp"

#include <iostream>
#include <memory>
#include <string>

struct OBJ : public Hittable {
  BVH bvh;

  OBJ(std::string &filepath, const std::shared_ptr<Material> &material) {
    objl::Loader Loader;
    if (!(Loader.LoadFile(filepath))) {
      std::cerr << "Error: OBJ path " << filepath << " is not a valid path\n";
      return;
    }
    std::cout << "Loading object " << filepath << "\n";

    HittableList objects;
    for (auto &mesh : Loader.LoadedMeshes) {
      for (int i = 0; i < mesh.Indices.size(); i += 3) {
        auto a = mesh.Vertices[mesh.Indices[i]].Position;
        auto b = mesh.Vertices[mesh.Indices[i + 1]].Position;
        auto c = mesh.Vertices[mesh.Indices[i + 2]].Position;
        auto ap = Point3(a.X, a.Y, a.Z);
        auto bp = Point3(b.X, b.Y, b.Z);
        auto cp = Point3(c.X, c.Y, c.Z);
        objects.pushHittable(std::make_shared<Triangle>(ap, bp, cp, material));
      }
    }

    bvh = BVH(objects);
  }

  bool hit(const Ray &ray,
           Hit &hit,
           const double minT,
           const double maxT) const override {
    return bvh.hit(ray, hit, minT, maxT);
  }

  [[nodiscard]] AABB boundingBox() const override { return bvh.boundingBox(); }
};

#endif //CPPTRACE_LIB_HITTABLE_OBJ_HPP_
