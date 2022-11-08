//
// Created by tyler on 11/4/2022.
//

#ifndef CPPTRACE_LIB_HITTABLE_OBJECTS_STL_HPP_
#define CPPTRACE_LIB_HITTABLE_OBJECTS_STL_HPP_

#include "../../common/vec3.hpp"
#include "../../accelerators/bvh.hpp"
#include "../materials/lambertian.hpp"
#include "../hittable.hpp"
#include "../hittable_list.hpp"
#include "triangle.hpp"

#include "../../extern/stl_reader.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct STL : public Hittable {
  HittableList hittableList;
  BVH bvh;

  STL(const std::string &filepath, const std::shared_ptr<Material> &material) {
    hittableList.clear();
    try {
      stl_reader::StlMesh<double, int> mesh(filepath);

      for (int i = 0; i < mesh.num_tris(); i++) {
        const double *a = mesh.vrt_coords(mesh.tri_corner_ind(i, 0));
        const double *b = mesh.vrt_coords(mesh.tri_corner_ind(i, 1));
        const double *c = mesh.vrt_coords(mesh.tri_corner_ind(i, 2));
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

  [[nodiscard]] std::vector<std::shared_ptr<Hittable>> getHittables() const override { return hittableList.hittables; }
};

#endif //CPPTRACE_LIB_HITTABLE_OBJECTS_STL_HPP_
