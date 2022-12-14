//
// Created by Tyler Hostler-Mathis on 10/18/22.
//

#ifndef CPPTRACE_LIB_HITTABLE_SPHERE_HPP_
#define CPPTRACE_LIB_HITTABLE_SPHERE_HPP_

#include "../../common/ray.hpp"
#include "../../common/vec3.hpp"
#include "../bounding/aabb.hpp"
#include "../materials/material.hpp"
#include "../hit.hpp"
#include "../hittable.hpp"

#include <climits>
#include <cmath>

struct Sphere : public Hittable {
  Point3 center;
  double radius, radiusSq;

  Sphere() : center(Point3(0, 0, 0)), radius(0), radiusSq(0) {}
  Sphere(const Point3 &center, const double radius,
         const std::shared_ptr<Material> &material)
      : center(center), radius(radius), radiusSq(radius * radius) {
    this->material = material;
  }

  [[nodiscard]] Hit hit(const Ray &ray,
           const double minT,
           const double maxT) const override {
    Vec3 oc = ray.origin - center;
    auto a = ray.direction.mag2();
    auto bHalf = oc.dot(ray.direction);
    auto c = oc.mag2() - radiusSq;
    auto discriminant = bHalf * bHalf - a * c;

    if (discriminant < 0)
      return {};

    auto sqrtD = sqrt(discriminant);
    double t = (-bHalf - sqrtD) / a;

    if (t < minT || t > maxT) {
      t = (-bHalf + sqrtD) / a;
      if (t < minT || t > maxT)
        return {};
    }

    Point3 location(ray.at(t));
    Normal normal((location - center) / radius);

    double u, v;
    calcUV(normal, u, v);

    bool front = ray.direction.dot(normal) < 0;
    if (!front)
      normal = -normal;

    return Hit(location, normal, this->material, t, u, v, front);
  }

  static void calcUV(const Point3 &location,
                     double &u,
                     double &v) {
    double theta = acos(-location.y);
    double phi = atan2(-location.z, location.x) + M_PI;

    u = phi / (2 * M_PI);
    v = theta / M_PI;
  }

  [[nodiscard]] AABB boundingBox() const override {
    return {center - Vec3(radius, radius, radius),
            center + Vec3(radius, radius, radius)};
  }
};

#endif //CPPTRACE_LIB_HITTABLE_SPHERE_HPP_
