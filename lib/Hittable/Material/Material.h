#ifndef MATERIAL_H
#define MATERIAL_H

#include "../../Ray.h"
#include "../../Vec3.h"

// Forward declaration to avoid dependency cycle
template <typename T> struct _Hit;

template <typename T> struct _Material {
  using Hit = _Hit<T>;
  using Ray = _Ray<T>;
  using Color3 = _Color3<T>;

  virtual void scatter(const Ray &in, const Hit &hit, Color3 &attenuation,
                       Ray &out) const = 0;
};

#endif
