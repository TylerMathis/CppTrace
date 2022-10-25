//
// Created by Tyler Hostler-Mathis on 10/24/22.
//

#ifndef RAYTRACER_LIB_HITTABLE_OBJECTS_RECTANGULAR_PRISM_HPP_
#define RAYTRACER_LIB_HITTABLE_OBJECTS_RECTANGULAR_PRISM_HPP_

#include "../../common/vec3.hpp"
#include "../hittable_list.hpp"
#include "../hittable.hpp"
#include "../hit.hpp"
#include "../materials/material.hpp"
#include "rectangle.hpp"

#include <memory>
#include <vector>

// Axis aligned prism. Configure translation and rotation with construction params
struct RectangularPrism : public Hittable {
  Point3 a, b;
  HittableList sides;

  RectangularPrism() = default;
  RectangularPrism(const Point3 &a,
                   const Point3 &b,
                   const std::shared_ptr<Material> &material) : a(a), b(b) {
    this->material = material;

    // Bottom and top
    sides.pushHittable(std::make_shared<Rectangle>(
        Triangle(Point3(b.x, a.y, a.z), a, Point3(a.x, a.y, b.z), material))
    );
    sides.pushHittable(std::make_shared<Rectangle>(
        Triangle(Point3(b.x, b.y, a.z), b, Point3(a.x, b.y, b.z), material))
    );

    // Left and right
    sides.pushHittable(std::make_shared<Rectangle>(
        Triangle(Point3(a.x, a.y, b.z), a, Point3(a.x, b.y, a.z), material))
    );
    sides.pushHittable(std::make_shared<Rectangle>(
        Triangle(Point3(b.x, a.y, b.z), b, Point3(b.x, b.y, a.z), material))
    );

    // Front and back
    sides.pushHittable(std::make_shared<Rectangle>(
        Triangle(Point3(b.x, a.y, a.z), a, Point3(a.x, b.y, a.z), material))
    );
    sides.pushHittable(std::make_shared<Rectangle>(
        Triangle(Point3(b.x, a.y, b.z), b, Point3(a.x, b.y, b.z), material))
    );
  }

  bool hit(const Ray &ray, Hit &hit, double minT, double maxT) const override {
    return sides.hit(ray, hit, minT, maxT);
  }

  [[nodiscard]] AABB boundingBox() const override {
    return sides.boundingBox();
  }
};

#endif //RAYTRACER_LIB_HITTABLE_OBJECTS_RECTANGULAR_PRISM_HPP_
