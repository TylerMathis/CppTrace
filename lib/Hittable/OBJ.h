#ifndef OBJ_H
#define OBJ_H

#include "../Vec3.h"
#include "./Bounding/BVH.h"
#include "./Material/Lambertian.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Triangle.h"

#include "../OBJ_Loader.h"

#include <climits>
#include <iostream>
#include <memory>
#include <string>

struct OBJ : public Hittable {
  BVH bvh;

  OBJ(std::string &filepath, const std::shared_ptr<Material> &material) {
    objl::Loader Loader;
    if (!(Loader.LoadFile(filepath))) {
      std::cerr << "Error: OBJ path " << filepath << " is not a valid path";
      return;
    }
    std::cout << "Loading object " << filepath << "\n";

    HittableList objects;
    for (auto &mesh : Loader.LoadedMeshes) {
      for (int j = 0; j < mesh.Indices.size(); j += 3) {
        auto a = mesh.Vertices[mesh.Indices[j]].Position;
        auto b = mesh.Vertices[mesh.Indices[j + 1]].Position;
        auto c = mesh.Vertices[mesh.Indices[j + 2]].Position;
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

#endif