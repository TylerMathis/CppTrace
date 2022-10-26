//
// Created by Tyler Hostler-Mathis on 10/24/22.
//

#ifndef RAYTRACER_LIB_HITTABLE_TEXTURES_TEXTURE_HPP_
#define RAYTRACER_LIB_HITTABLE_TEXTURES_TEXTURE_HPP_

#include "../../common/vec3.hpp"

struct Texture {
  [[nodiscard]] virtual Color3 value(double u,
                                     double v,
                                     const Point3 &point) const = 0;
};

#endif //RAYTRACER_LIB_HITTABLE_TEXTURES_TEXTURE_HPP_
