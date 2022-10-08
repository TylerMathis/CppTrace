#ifndef HITTABLE_H
#define HITTABLE_H

#include "../Ray.h"
#include "./Material/Material.h"
#include "Hit.h"

#include "limits.h"

template <typename T, int minT = 0, int maxT = INT_MAX> struct _Hittable {
  using Ray = _Ray<T>;
  using Material = _Material<T>;
  using Hit = _Hit<T>;

  Material *material;

  virtual bool rayCast(const Ray &ray, Hit &out) const = 0;

  bool validT(const double t) const {
    return t >= (minT + 0.0001) && t <= maxT;
  }
};

#endif
