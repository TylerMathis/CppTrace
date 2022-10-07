#ifndef METAL_H
#define METAL_H

#include "../../Ray.h"
#include "../../Vec3.h"
#include "../Hit.h"
#include "Material.h"

template <typename T> struct Metal : public Material<T> {
  using Ray = Ray<T>;
  using Vec3 = Vec3<T>;
  using Point3 = Point3<T>;
  using Color3 = Color3<T>;
  using Hit = Hit<T>;

  Color3 albedo;

  Metal(const Color3 &albedo) : albedo(albedo) {}

  virtual void scatter(const Ray &in, const Hit &hit, Color3 &attenuation,
                       Ray &out) const override {
    Vec3 reflected = in.direction.unit().reflect(hit.normal);
    out = Ray(hit.location, reflected);
    attenuation = albedo;
  }
};

#endif