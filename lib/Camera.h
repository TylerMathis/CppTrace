#ifndef CAMERA_H
#define CAMERA_H

#include "./Hittable/Hittable.h"
#include "Ray.h"
#include "Vec3.h"

#include <vector>

#include "math.h"

template <typename T> struct Camera {
  using Ray = Ray<T>;
  using Vec3 = Vec3<T>;
  using Point3 = Point3<T>;
  using Color3 = Color3<T>;
  using Normal = Normal<T>;

  double viewportWidth, viewportHeight;
  Point3 origin;

  int samples, bounceDepth;

  Camera(const Point3 origin, const Point3 lookAt, const Vec3 up,
         const double verticalFOV, const double aspectRatio,
         const int samples = 1, const int bounceDepth = 10)
      : origin(origin), samples(samples), bounceDepth(bounceDepth) {
    double theta = verticalFOV / 180 * M_PI;
    double h = tan(theta / 2);
    viewportHeight = 2.0 * h;
    viewportWidth = aspectRatio * viewportHeight;

    Vec3 w = (origin - lookAt).unit();
    Vec3 u = (up.cross(w)).unit();
    Vec3 v = w.cross(u);

    horizontal = u * viewportWidth;
    vertical = v * -viewportHeight;
    topLeftCorner = origin - horizontal / 2 - vertical / 2 - w;
  }
  Camera() : Camera(Point3(0, 0, 0), Point3(0, 0, -1), Vec3(0, 1, 0), 60, 1) {}

  Ray getRay(const double x, const double y) const {
    return Ray(origin, topLeftCorner + horizontal * x + vertical * y - origin);
  }

private:
  Point3 topLeftCorner;
  Vec3 horizontal, vertical;
};

#endif