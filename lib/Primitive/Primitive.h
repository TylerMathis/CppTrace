#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "../Ray.h"
#include "../Vec3.h"

template <typename T = double> struct Primitive {
  using Ray = Ray<>;
  using Normal = Normal<>;

  virtual bool rayHit(const Ray &r, Normal &out) const { return false; };
};

#endif