#ifndef METAL_H
#define METAL_H

#include "../../Common.h"
#include "../../Ray.h"
#include "../../Vec3.h"
#include "../Hit.h"
#include "Material.h"

template <typename T> struct _Metal : public _Material<T> {
  Color3 albedo;
  double fuzziness;

  _Metal() : albedo(), fuzziness(0.0) {}
  _Metal(const Color3 &albedo, double fuzziness = 0)
      : albedo(albedo), fuzziness(common::clamp(fuzziness, 0, 1)) {}

  virtual void scatter(const Ray &in, const Hit &hit, Color3 &attenuation,
                       Ray &out) const override {
    Vec3 reflected = in.direction.unit().reflect(hit.normal);
    out = Ray(hit.location, reflected + Vec3::randomInUnitSphere() * fuzziness);
    attenuation = albedo;
  }
};

using Metal = _Metal<double>;

#endif
