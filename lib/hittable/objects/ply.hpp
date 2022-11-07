//
// Created by tyler on 11/4/2022.
//

#ifndef CPPTRACE_LIB_HITTABLE_OBJECTS_PLY_HPP_
#define CPPTRACE_LIB_HITTABLE_OBJECTS_PLY_HPP_

#include "../../common/vec3.hpp"
#include "../../accelerators/bvh.hpp"
#include "../materials/lambertian.hpp"
#include "../hittable.hpp"
#include "../hittable_list.hpp"
#include "triangle.hpp"

#include "../../extern/happly.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <array>

struct PLY : public Hittable {
  HittableList hittableList;
  BVH bvh;

  PLY(const std::string &filepath, const std::shared_ptr<Material> &material) {
    hittableList.clear();
    try {
      happly::PLYData mesh(filepath);

      std::vector<std::array<double, 3>> vertices = mesh.getVertexPositions();
      std::vector<std::vector<int>> triangles = mesh.getFaceIndices<int>();

      for (const auto &triangle : triangles) {
        auto a = vertices[triangle[0]];
        auto b = vertices[triangle[1]];
        auto c = vertices[triangle[2]];
        auto ap = Point3(a[0], a[1], a[2]);
        auto bp = Point3(b[0], b[1], b[2]);
        auto cp = Point3(c[0], c[1], c[2]);
        hittableList.pushHittable(std::make_shared<Triangle>(ap, bp, cp, material));
      }

    } catch (std::exception &e) {
      std::cerr << e.what() << std::endl;
    }

    bvh = BVH(hittableList);
  }

  bool hit(const Ray &ray,
           Hit &hit,
           const double minT,
           const double maxT) const override {
    return bvh.hit(ray, hit, minT, maxT);
  }

  [[nodiscard]] AABB boundingBox() const override { return bvh.boundingBox(); }
};

#endif //CPPTRACE_LIB_HITTABLE_OBJECTS_PLY_HPP_
