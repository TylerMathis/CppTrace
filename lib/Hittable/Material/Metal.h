#ifndef METAL_H
#define METAL_H

#include "../../Common.h"
#include "../../Ray.h"
#include "../../Vec3.h"
#include "../Hit.h"
#include "Material.h"

struct Metal : public Material {
  Color3 albedo;
  double fuzziness;

  Metal() : albedo(), fuzziness(0) {}
  Metal(const Color3 &albedo, double fuzziness = 0)
      : albedo(albedo), fuzziness(common::clamp(fuzziness, 0, 1)) {}

  virtual void scatter(const Ray &in, const Hit &hit, Color3 &attenuation,
                       Ray &out) const override {
    Vec3 reflected = in.direction.unit().reflect(hit.normal);
    out = Ray(hit.location, reflected + Vec3::randomInUnitSphere() * fuzziness);
    attenuation = albedo;
  }
};

#endif
