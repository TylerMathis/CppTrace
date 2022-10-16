#ifndef MATERIAL_H
#define MATERIAL_H

#include "../../Ray.h"
#include "../../Vec3.h"

// Forward declaration to avoid dependency cycle
struct Hit;

struct Material {
  virtual void scatter(const Ray &in, const Hit &hit, Color3 &attenuation,
                       Ray &out) const = 0;
};

#endif
