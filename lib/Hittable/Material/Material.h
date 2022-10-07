#ifndef MATERIAL_H
#define MATERIAL_H

#include "../../Ray.h"
#include "../../Vec3.h"

// Forward declaration to avoid dependency cycle
template <typename T> struct Hit;

template <typename T> struct Material {
  using Hit = Hit<T>;
  using Ray = Ray<T>;
  using Color3 = Color3<T>;

  virtual void scatter(const Ray &in, const Hit &hit, Color3 &attenuation,
                       Ray &out) const = 0;
};

#endif