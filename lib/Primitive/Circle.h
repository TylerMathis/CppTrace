#ifndef CIRCLE_H
#define CIRCLE_H

#include "../Ray.h"
#include "../Vec3.h"
#include "Primitive.h"

template <typename T = double> struct Circle : public Primitive<T> {
  using Ray = Ray<>;
  using Vec3 = Vec3<>;
  using Point3 = Point3<>;
  using Normal = Normal<>;

  Point3 center;
  double radius, radiusSq;

  Circle() : center(Point3(0, 0, 0)), radius(0), radiusSq(0) {}
  Circle(const Point3 &center, const double radius)
      : center(center), radius(radius), radiusSq(radius * radius) {}

  // Returns true if hit, and populates out with the normal at hit location
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