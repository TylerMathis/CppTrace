#ifndef OBJ_H
#define OBJ_H

#include "../Vec3.h"
#include "./Bounding/BVH.h"
#include "./Material/Lambertian.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Triangle.h"

#include "../OBJ_Loader.h"

#include "limits.h"

#include <iostream>
#include <memory>
#include <string>

template <typename T, int minT = 0, int maxT = INT_MAX>
struct _OBJ : public _Hittable<T, minT, maxT> {
  BVH bvh;

  _OBJ(std::string &filepath, std::shared_ptr<Material> material) {
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

  virtual bool hit(const Ray &ray, Hit &hit) const override {
    return bvh.hit(ray, hit);
  }

  virtual AABB boundingBox() const override { return bvh.boundingBox(); }
};

using OBJ = _OBJ<double>;

#endif