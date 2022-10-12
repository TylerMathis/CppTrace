#ifndef CAMERA_H
#define CAMERA_H

#include "./Hittable/Hittable.h"
#include "Ray.h"
#include "Vec3.h"

#include <vector>

#include "math.h"

template <typename T> struct _Camera {
  double viewportWidth, viewportHeight;
  Point3 origin;

  _Camera(const Point3 origin, const Point3 lookAt, const Vec3 up,
          const double verticalFOV, const double aspectRatio,
          const double aperture, const double focusDist)
      : origin(origin) {
    double theta = verticalFOV / 180 * M_PI;
    double h = tan(theta / 2);
    viewportHeight = 2.0 * h;
    viewportWidth = aspectRatio * viewportHeight;

    w = (origin - lookAt).unit();
    u = (up.cross(w)).unit();
    v = w.cross(u);

    horizontal = u * viewportWidth * focusDist;
    vertical = v * -viewportHeight * focusDist;
    topLeftCorner = origin - horizontal / 2 - vertical / 2 - w * focusDist;

    lensRadius = aperture / 2;
  }
  _Camera()
      : _Camera(Point3(0, 0, 0), Point3(0, 0, -1), Vec3(0, 1, 0), 60, 1) {}

  Ray getRay(const double x, const double y) const {
    Vec3 rd = Vec3::randomInUnitDisk() * lensRadius;
    Vec3 offset = u * rd.x + v * rd.y;
    return Ray(origin + offset,
               topLeftCorner + horizontal * x + vertical * y - origin - offset);
  }

private:
  Point3 topLeftCorner;
  Vec3 horizontal, vertical;
  Vec3 u, v, w;
  double lensRadius;
};

using Camera = _Camera<double>;

#endif
