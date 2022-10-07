#ifndef HITTABLE_H
#define HITTABLE_H

#include "../Hit.h"
#include "../Ray.h"

#include "limits.h"

template <typename T, int minT = 0, int maxT = INT_MAX> struct Hittable {
  using Ray = Ray<T>;
  using Hit = Hit<T>;

  virtual bool rayHit(const Ray &r, Hit &out) const = 0;

  bool validT(const double t) const { return t >= minT && t <= maxT; }
};

#endif