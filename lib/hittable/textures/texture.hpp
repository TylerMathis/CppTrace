//
// Created by Tyler Hostler-Mathis on 10/24/22.
//

#ifndef CPPTRACE_LIB_HITTABLE_TEXTURES_TEXTURE_HPP_
#define CPPTRACE_LIB_HITTABLE_TEXTURES_TEXTURE_HPP_

#include "../../common/vec3.hpp"

struct Texture {
  [[nodiscard]] virtual Color3 value(double u,
                                     double v,
                                     const Point3 &point) const = 0;
};

#endif //CPPTRACE_LIB_HITTABLE_TEXTURES_TEXTURE_HPP_
