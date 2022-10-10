#ifndef METAL_H
#define METAL_H

#include "../../Ray.h"
#include "../../Vec3.h"
#include "../Hit.h"
#include "Material.h"

template <typename T> struct _Metal : public _Material<T> {
  Color3 albedo;

  _Metal(const Color3 &albedo) : albedo(albedo) {}

  virtual void scatter(const Ray &in, const Hit &hit, Color3 &attenuation,
                       Ray &out) const override {
    Vec3 reflected = in.direction.unit().reflect(hit.normal);
    out = Ray(hit.location, reflected);
    attenuation = albedo;
  }
};

using Metal = _Metal<double>;

#endif
