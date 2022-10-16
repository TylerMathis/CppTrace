#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include "../../Ray.h"
#include "../../Vec3.h"
#include "../Hit.h"
#include "Material.h"

struct Lambertian : public Material {
  Color3 albedo;

  explicit Lambertian(const Color3 &albedo) : albedo(albedo) {}

  void scatter(const Ray &in, const Hit &hit, Color3 &attenuation,
               Ray &out) const override {
    Point3 target = hit.location + Vec3::randomInHemisphere(hit.normal);
    out = Ray(hit.location, target - hit.location);
    attenuation = albedo;
  }
};

#endif
