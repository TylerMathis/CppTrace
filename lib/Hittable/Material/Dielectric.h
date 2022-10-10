#ifndef DIELECTRIC_H
#define DIELECTRIC_H

#include "../../Ray.h"
#include "../../Vec3.h"
#include "../Hit.h"
#include "Material.h"

template <typename T> struct _Dielectric : public _Material<T> {
  double refractionIndex;

  _Dielectric(const double refractionIndex)
      : refractionIndex(refractionIndex) {}

  virtual void scatter(const Ray &in, const Hit &hit, Color3 &attenuation,
                       Ray &out) const override {
    attenuation = Color3(1, 1, 1);

    double refractionRatio = hit.front ? 1 / refractionIndex : refractionIndex;
    Vec3 unitDirection = in.direction.unit();
    double cosTheta = std::min(-unitDirection.dot(hit.normal), 1.0);
    double sinTheta = sqrt(1 - cosTheta * cosTheta);

    bool invalidRefraction = refractionRatio * sinTheta > 1;
    bool shouldRefract =
        !invalidRefraction &&
        reflectance(cosTheta, refractionRatio) <= common::randomDouble();

    Vec3 outDirection = shouldRefract
                            ? unitDirection.refract(hit.normal, refractionRatio)
                            : unitDirection.reflect(hit.normal);

    out = Ray(hit.location, outDirection);
  }

private:
  // Schlick's approx
  double reflectance(const double cosTheta,
                     const double refractionRatio) const {
    double r0 = (1 - refractionRatio) / (1 + refractionRatio);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosTheta), 5);
  }
};

using Dielectric = _Dielectric<double>;

#endif
