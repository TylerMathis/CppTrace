#ifndef MATERIAL_H
#define MATERIAL_H

#include "../../Ray.h"
#include "../../Vec3.h"

// Forward declaration to avoid dependency cycle
template <typename T> struct _Hit;
using Hit = _Hit<double>;

template <typename T> struct _Material {
  virtual void scatter(const Ray &in, const Hit &hit, Color3 &attenuation,
                       Ray &out) const = 0;
};

using Material = _Material<double>;

#endif
