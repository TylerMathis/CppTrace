#ifndef SPHERE_H
#define SPHERE_H

#include "../Ray.h"
#include "../Vec3.h"
#include "Primitive.h"

template <typename T> struct Sphere : public Primitive<T> {
  using Ray = Ray<T>;
  using Vec3 = Vec3<T>;
  using Point3 = Point3<T>;
  using Normal = Normal<T>;

  Point3 center;
  double radius, radiusSq;

  Sphere() : center(Point3(0, 0, 0)), radius(0), radiusSq(0) {}
  Sphere(const Point3 &center, const double radius)
      : center(center), radius(radius), radiusSq(radius * radius) {}

  bool rayHit(const Ray &r, Normal &out) const {
    Vec3 oc = r.orig - center;
    auto a = r.dir.dot(r.dir);
    auto b = oc.dot(r.dir) * 2.0;
    auto c = oc.dot(oc) - radiusSq;
    auto discriminant = b * b - 4 * a * c;

    // On the line and in front of origin
    bool hit = discriminant > 0;

    double t = (-b - sqrt(discriminant)) / (2.0 * a);
    hit &= t > 0;

    if (hit) {
      Normal N = (r.at(t) - center).unit();
      out = N;
      return true;
    }

    return false;
  }
};

#endif