#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "../Hit.h"
#include "../Ray.h"

template <typename T> struct Primitive {
  using Ray = Ray<T>;
  using Hit = Hit<T>;

  // Returns true if hit, and populates out with a valid Hit
  virtual bool rayHit(const Ray &r, Hit &out) const = 0;
};

#endif