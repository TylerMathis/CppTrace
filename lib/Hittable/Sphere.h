#ifndef SPHERE_H
#define SPHERE_H

#include "../Hit.h"
#include "../Ray.h"
#include "../Vec3.h"
#include "Hittable.h"

#include "limits.h"

template <typename T, int minT = 0, int maxT = INT_MAX>
struct Sphere : public Hittable<T, minT, maxT> {
  using Hit = Hit<T>;
  using Ray = Ray<T>;
  using Vec3 = Vec3<T>;
  using Point3 = Point3<T>;
  using Normal = Normal<T>;

  Point3 center;
  double radius, radiusSq;

  Sphere() : center(Point3(0, 0, 0)), radius(0), radiusSq(0) {}
  Sphere(const Point3 &center, const double radius)
      : center(center), radius(radius), radiusSq(radius * radius) {}

  bool rayHit(const Ray &r, Hit &out) const override {
    Vec3 oc = r.orig - center;
    auto a = r.dir.mag2();
    auto bHalf = oc.dot(r.dir);
    auto c = oc.mag2() - radiusSq;
    auto discriminant = bHalf * bHalf - a * c;

    // behind us
    if (discriminant < 0)
      return false;

    auto sqrtD = sqrt(discriminant);
    double t = (-bHalf - sqrtD) / a;

    // test both intersections for between clip plane
    if (!this->validT(t)) {
      t = (-bHalf + sqrtD) / a;
      if (!this->validT(t))
        return false;
    }

    Point3 loc(r.at(t));
    Normal normal((loc - center) / radius);
    out = Hit(loc, normal, t);

    return true;
  }
};

#endif