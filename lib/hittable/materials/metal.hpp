//
// Created by Tyler Hostler-Mathis on 10/18/22.
//

#ifndef CPPTRACE_LIB_HITTABLE_MATERIAL_METAL_HPP_
#define CPPTRACE_LIB_HITTABLE_MATERIAL_METAL_HPP_

#include "../../common/common.hpp"
#include "../../common/ray.hpp"
#include "../../common/vec3.hpp"
#include "../hit.hpp"
#include "material.hpp"
#include "../textures/texture.hpp"

#include <memory>
#include <utility>

struct Metal : public Material {
  std::shared_ptr<Texture> texture;
  double fuzziness;

  Metal() = default;
  explicit Metal(std::shared_ptr<Texture> texture, double fuzziness = 0)
      : texture(std::move(texture)),
        fuzziness(common::clamp(fuzziness, 0, 1)) {}

  bool scatter(const Ray &in, const Hit &hit, Color3 &attenuation,
               Ray &out) const override {
    Vec3 reflected = in.direction.unit().reflect(hit.normal);
    out = Ray(hit.location, reflected + Vec3::randomInUnitSphere() * fuzziness);
    attenuation = texture->value(hit.u, hit.v, hit.location);
    return true;
  }
};

#endif //CPPTRACE_LIB_HITTABLE_MATERIAL_METAL_HPP_
