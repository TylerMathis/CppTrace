#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "../Ray.h"
#include "../Vec3.h"
#include "./Bounding/AABB.h"
#include "./Material/Material.h"
#include "Hit.h"
#include "Hittable.h"

#include "limits.h"

#include <memory>
#include <vector>

// CCW Triangle
template <typename T, int minT = 0, int maxT = INT_MAX>
struct _Triangle : public _Hittable<T, minT, maxT> {
  const double EPS = 1e-6;

  Point3 a, b, c;
  std::vector<Point3> points;
  Normal normal;

  _Triangle(Point3 &a, Point3 &b, Point3 &c, std::shared_ptr<Material> material)
      : a(a), b(b), c(c),
        // Wrap point to avoid modulo
        points({a, b, c, a}), normal((b - a).cross(c - a).unit()) {
    this->material = material;
  }

  virtual bool hit(const Ray &ray, Hit &hit) const override {
    // If ray is parallel to triangle plane, no hit
    double normalDotRayDir = normal.dot(ray.direction);
    if (std::abs(normalDotRayDir) <= EPS)
      return false;

    double d = -normal.dot(a);
    double t = -(normal.dot(ray.origin) + d) / normalDotRayDir;

    if (!this->validT(t))
      return false;

    Vec3 location = ray.at(t);

    for (int i = 0; i < 3; i++) {
      Vec3 edge = points[i + 1] - points[i];
      Vec3 vecToLocation = location - points[i];
      Vec3 inFacing = edge.cross(vecToLocation);
      if (normal.dot(inFacing) < 0)
        return false;
    }

    bool front = ray.direction.dot(normal) < 0;
    Normal hitNormal = front ? normal : -normal;
    hit = Hit(location, hitNormal, this->material, t, front);

    return true;
  }

  virtual AABB boundingBox() const override {
    Point3 minPoint = Point3(std::min(std::min(a.x, b.x), c.x),
                             std::min(std::min(a.y, b.y), c.y),
                             std::min(std::min(a.z, b.z), c.z));
    Point3 maxPoint = Point3(std::max(std::max(a.x, b.x), c.x),
                             std::max(std::max(a.y, b.y), c.y),
                             std::max(std::max(a.z, b.z), c.z));

    if (minPoint.x == maxPoint.x)
      minPoint.x -= 0.001, maxPoint.x += 0.001;
    if (minPoint.y == maxPoint.y)
      minPoint.y -= 0.001, maxPoint.y += 0.001;
    if (minPoint.z == maxPoint.z)
      minPoint.z -= 0.001, maxPoint.z += 0.001;

    return AABB(minPoint - normal.abs(), maxPoint + normal.abs());
  }
};

using Triangle = _Triangle<double>;

#endif