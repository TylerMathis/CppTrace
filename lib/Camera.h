#ifndef CAMERA_H
#define CAMERA_H

#include "./Hittable/Hittable.h"
#include "Ray.h"
#include "Vec3.h"

#include <vector>

template <typename T> struct Camera {
  using Ray = Ray<T>;
  using Vec3 = Vec3<T>;
  using Point3 = Point3<T>;
  using Color3 = Color3<T>;
  using Normal = Normal<T>;

  double viewportWidth, viewportHeight, focalLength;
  Point3 origin;

  int samples;

  void calcVectors() {
    horizontal = Vec3(viewportWidth, 0, 0);
    vertical = Vec3(0, -viewportHeight, 0);
    topLeftCorner =
        origin - horizontal / 2 - vertical / 2 - Vec3(0, 0, focalLength);
  }

  Camera()
      : viewportWidth(100), viewportHeight(100), focalLength(1),
        origin(Point3(0, 0, 0)), samples(1) {
    calcVectors();
  }

  Camera(const double viewportWidth, const double viewportHeight,
         const double focalLength, const Point3 origin = Point3(0, 0, 0),
         const int samples = 1)
      : viewportWidth(viewportWidth), viewportHeight(viewportHeight),
        focalLength(focalLength), origin(origin), samples(samples) {
    calcVectors();
  }

  Ray getRay(const double x, const double y) const {
    return Ray(origin, topLeftCorner + horizontal * x + vertical * y - origin);
  }

private:
  Point3 topLeftCorner;
  Vec3 horizontal, vertical;
};

#endif