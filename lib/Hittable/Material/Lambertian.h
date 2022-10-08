#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include "../../Ray.h"
#include "../../Vec3.h"
#include "../Hit.h"
#include "Material.h"

template <typename T> struct _Lambertian : public _Material<T> {
  using Ray = _Ray<T>;
  using Vec3 = _Vec3<T>;
  using Point3 = _Point3<T>;
  using Color3 = _Color3<T>;
  using Hit = _Hit<T>;

  Color3 albedo;

  _Lambertian(const Color3 &albedo) : albedo(albedo) {}

  virtual void scatter(const Ray &in, const Hit &hit, Color3 &attenuation,
                       Ray &out) const override {
    Point3 target = hit.location + Vec3::randomInHemisphere(hit.normal);
    out = Ray(hit.location, target - hit.location);
    attenuation = albedo;
  }
};

#endif
