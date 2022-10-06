#ifndef CAMERA_H
#define CAMERA_H

#include "./Primitive/Primitive.h"
#include "Ray.h"
#include "Vec3.h"

#include <vector>

template <typename T> struct Camera {
  using Primitive = Primitive<T>;
  using Ray = Ray<T>;
  using Vec3 = Vec3<T>;
  using Point3 = Point3<T>;
  using Color3 = Color3<T>;
  using Normal = Normal<T>;

  double viewportWidth, viewportHeight, focalLength;
  Point3 origin;

  std::vector<Primitive *> primitives;

  void calcVectors() {
    horizontal = Vec3(viewportWidth, 0, 0);
    vertical = Vec3(0, viewportHeight, 0);
    lowerLeftCorner =
        origin - horizontal / 2 - vertical / 2 - Vec3(0, 0, focalLength);
  }

  Camera()
      : viewportWidth(100), viewportHeight(100), focalLength(1),
        origin(Point3(0, 0, 0)) {
    calcVectors();
  }

  Camera(const double viewportWidth, const double viewportHeight,
         const double focalLength, const Point3 origin = Point3(0, 0, 0))
      : viewportWidth(viewportWidth), viewportHeight(viewportHeight),
        focalLength(focalLength), origin(origin) {
    calcVectors();
  }

  void pushPrimitive(Primitive *p) { primitives.push_back(p); }

  Ray getRay(const double x, const double y) const {
    return Ray(origin,
               lowerLeftCorner + horizontal * x + vertical * y - origin);
  }

  Color3 getRayColor(const Ray &r) const {
    Normal out;
    for (Primitive *p : primitives) {
      if (p->rayHit(r, out)) {
        return (Color3(out) + 1) / 2;
      }
    }

    Vec3 unitDir = r.dir.unit();
    double t = (unitDir.y + 1.0) / 2;
    return Color3(1, 1, 1) * (1.0 - t) + Color3(0.5, 0.7, 1.0) * t;
  }

  // Get the color of the pixel 1/x to the right and 1/y up
  Color3 getColor(const double x, const double y) const {
    return getRayColor(getRay(x, y));
  }

private:
  Vec3 horizontal, vertical, lowerLeftCorner;
};

#endif