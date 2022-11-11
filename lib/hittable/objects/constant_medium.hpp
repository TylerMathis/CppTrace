//
// Created by Tyler Hostler-Mathis on 10/25/22.
//

#ifndef CPPTRACE_LIB_HITTABLE_OBJECTS_CONSTANT_MEDIUM_HPP_
#define CPPTRACE_LIB_HITTABLE_OBJECTS_CONSTANT_MEDIUM_HPP_

#include "../hittable.hpp"
#include "../hit.hpp"
#include "../materials/material.hpp"
#include "../textures/texture.hpp"
#include "../materials/isotropic.hpp"
#include "../../common/ray.hpp"
#include "../../common/vec3.hpp"
#include "../../common/common.hpp"

#include <memory>
#include <utility>
#include <cfloat>

struct ConstantMedium : public Hittable {
  std::shared_ptr<Hittable> boundary;
  std::shared_ptr<Material> phaseFunction;
  double negativeInverseDensity;

  ConstantMedium(std::shared_ptr<Hittable> boundary,
                 const double density,
                 const std::shared_ptr<Texture> &phaseFunction)
      : boundary(std::move(boundary)),
        negativeInverseDensity(-1.0 / density),
        phaseFunction(std::make_shared<Isotropic>(phaseFunction)) {}

  ConstantMedium(std::shared_ptr<Hittable> boundary,
                 const double density,
                 const Color3 &color)
      : boundary(std::move(boundary)),
        negativeInverseDensity(-1.0 / density),
        phaseFunction(std::make_shared<Isotropic>(color)) {}

  Hit hit(const Ray &ray, double minT, double maxT) const override {
    Hit entry, exit;

    entry = boundary->hit(ray, minT, maxT);
    if (!entry.valid)
      return {};

    exit = boundary->hit(ray, entry.t + 0.001, maxT);
    if (!exit.valid)
      return {};

    entry.t = std::max(entry.t, minT);
    exit.t = std::min(exit.t, maxT);

    if (entry.t >= exit.t)
      return {};

    entry.t = std::max(entry.t, 0.0);

    const double rayLength = ray.direction.mag();
    const double intersectionLength = (exit.t - entry.t) * rayLength;
    const double
        distToHit = negativeInverseDensity * std::log(common::randomDouble());

    if (distToHit > intersectionLength)
      return {};

    const double t = entry.t + distToHit / rayLength;
    return { ray.at(t), Vec3(1, 0, 0), phaseFunction, t, 0, 0, true };
  }

  [[nodiscard]] AABB boundingBox() const override {
    return boundary->boundingBox();
  }
};

#endif //CPPTRACE_LIB_HITTABLE_OBJECTS_CONSTANT_MEDIUM_HPP_
