#ifndef CAMERA_H
#define CAMERA_H

#include "Vec3.h"

struct Camera {
  int viewportWidth, viewportHeight, focalLength;
  Point3 origin;

  Camera()
      : viewportWidth(100), viewportHeight(100), focalLength(1),
        origin(0, 0, 0) {}

  Camera(const int viewportWidth, const int viewportHeight,
         const int focalLength, const Point3 origin = Point3(0, 0, 0))
      : viewportWidth(viewportWidth), viewportHeight(viewportHeight),
        focalLength(focalLength), origin(origin) {}
};

#endif