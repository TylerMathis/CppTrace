#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "../Ray.h"
#include "../Vec3.h"

template <typename T> struct Primitive {
  using Ray = Ray<T>;
  using Normal = Normal<T>;

  // Returns true if hit, and populates out with the normal at hit location
  virtual bool rayHit(const Ray &r, Normal &out) const { return false; };
};

#endif