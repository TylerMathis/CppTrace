//
// Created by Tyler Hostler-Mathis on 10/18/22.
//

#ifndef CPPTRACE_LIB_HITTABLE_TRIANGLE_HPP_
#define CPPTRACE_LIB_HITTABLE_TRIANGLE_HPP_

#include "../../common/ray.hpp"
#include "../../common/vec3.hpp"
#include "../bounding/aabb.hpp"
#include "../materials/material.hpp"
#include "../hit.hpp"
#include "../hittable.hpp"

#include <climits>
#include <memory>
#include <vector>

// CCW Triangle
struct Triangle : public Hittable {
  const double EPS = 1e-6;

  Point3 a, b, c;
  std::vector<Point3> points;
  Normal normal;

  Triangle() = default;
  Triangle(const Point3 &a,
           const Point3 &b,
           const Point3 &c,
           const std::shared_ptr<Material> &material)
      : a(a), b(b), c(c),
      // Wrap point to avoid modulo
        points({a, b, c, a}), normal((b - a).cross(c - a).unit()) {
    this->material = material;
  }

  [[nodiscard]] Hit hit(const Ray &ray,
                        const double minT,
                        const double maxT) const override {
    // If ray is parallel to triangle plane, no hit
    double normalDotRayDir = normal.dot(ray.direction);
    if (std::abs(normalDotRayDir) <= EPS)
      return {};

    double d = -normal.dot(a);
    double t = -(normal.dot(ray.origin) + d) / normalDotRayDir;

    if (t < minT || t > maxT)
      return {};

    Vec3 location = ray.at(t);

    for (int i = 0; i < 3; i++) {
      Vec3 edge = points[i + 1] - points[i];
      Vec3 vecToLocation = location - points[i];
      Vec3 inFacing = edge.cross(vecToLocation);
      if (normal.dot(inFacing) < 0)
        return {};
    }

    bool front = ray.direction.dot(normal) < 0;
    Normal hitNormal = front ? normal : -normal;

    // TODO: Use actual uvs
    return {location, hitNormal, this->material, t, 0, 0, front};
  }

  [[nodiscard]] AABB boundingBox() const override {
    Point3 minPoint = Point3(std::min(std::min(a.x, b.x), c.x),
                             std::min(std::min(a.y, b.y), c.y),
                             std::min(std::min(a.z, b.z), c.z));
    Point3 maxPoint = Point3(std::max(std::max(a.x, b.x), c.x),
                             std::max(std::max(a.y, b.y), c.y),
                             std::max(std::max(a.z, b.z), c.z));

    if (minPoint.x == maxPoint.x)
      minPoint.x -= 0.001, maxPoint.x += 0.001;
    if (minPoint.y == maxPoint.y)
      minPoint.y -= 0.001, maxPoint.y += 0.001;
    if (minPoint.z == maxPoint.z)
      minPoint.z -= 0.001, maxPoint.z += 0.001;

    return {minPoint - normal.abs(), maxPoint + normal.abs()};
  }
};

#endif //CPPTRACE_LIB_HITTABLE_TRIANGLE_HPP_
