//
// Created by Tyler Hostler-Mathis on 10/18/22.
//

#ifndef RAYTRACER_LIB_HITTABLE_MATERIAL_DIELECTRIC_HPP_
#define RAYTRACER_LIB_HITTABLE_MATERIAL_DIELECTRIC_HPP_

#include "../../common/ray.hpp"
#include "../../common/vec3.hpp"
#include "../hit.hpp"
#include "material.hpp"

struct Dielectric : public Material {
  double refractionIndex;

  Dielectric() : refractionIndex(0) {}
  explicit Dielectric(const double refractionIndex)
      : refractionIndex(refractionIndex) {}

  bool scatter(const Ray &in, const Hit &hit, Color3 &attenuation,
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
    return true;
  }

 private:
  // Schlick's approx
  static double reflectance(const double cosTheta,
                            const double refractionRatio) {
    double r0 = (1 - refractionRatio) / (1 + refractionRatio);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosTheta), 5);
  }
};

#endif //RAYTRACER_LIB_HITTABLE_MATERIAL_DIELECTRIC_HPP_
