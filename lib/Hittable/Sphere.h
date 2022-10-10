#ifndef SPHERE_H
#define SPHERE_H

#include "../Ray.h"
#include "../Vec3.h"
#include "./Material/Material.h"
#include "Hit.h"
#include "Hittable.h"

#include "limits.h"

template <typename T, int minT = 0, int maxT = INT_MAX>
struct _Sphere : public _Hittable<T, minT, maxT> {
  Point3 center;
  double radius, radiusSq;

  _Sphere() : center(Point3(0, 0, 0)), radius(0), radiusSq(0) {}
  _Sphere(const Point3 &center, const double radius, Material *material)
      : center(center), radius(radius), radiusSq(radius * radius) {
    this->material = material;
  }

  virtual bool rayCast(const Ray &ray, Hit &out) const override {
    Vec3 oc = ray.origin - center;
    auto a = ray.direction.mag2();
    auto bHalf = oc.dot(ray.direction);
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

    Point3 location(ray.at(t));
    Normal normal((location - center) / radius);
    out = Hit(location, normal, this->material, t);

    return true;
  }
};

using Sphere = _Sphere<double>;

#endif
