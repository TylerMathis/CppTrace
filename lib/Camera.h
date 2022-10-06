#ifndef CAMERA_H
#define CAMERA_H

#include "Vec3.h"

struct Camera {
  int viewportWidth, viewportHeight, focalLength;
  Point3 origin;

  void calcVectors() {
    horizontal = Vec3(viewportWidth, 0, 0);
    vertical = Vec3(0, viewportHeight, 0);
    lowerLeftCorner =
        origin - horizontal / 2 - vertical / 2 - Vec3(0, 0, focalLength);
  }

  Camera()
      : viewportWidth(100), viewportHeight(100), focalLength(1),
        origin(0, 0, 0) {
    calcVectors();
  }

  Camera(const int viewportWidth, const int viewportHeight,
         const int focalLength, const Point3 origin = Point3(0, 0, 0))
      : viewportWidth(viewportWidth), viewportHeight(viewportHeight),
        focalLength(focalLength), origin(origin) {
    calcVectors();
  }

private:
  Vec3 horizontal, vertical, lowerLeftCorner;
};

#endif