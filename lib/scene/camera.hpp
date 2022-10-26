//
// Created by Tyler Hostler-Mathis on 10/18/22.
//

#ifndef RAYTRACER_LIB_SCENE_CAMERA_HPP_
#define RAYTRACER_LIB_SCENE_CAMERA_HPP_

#include "../hittable/hittable.hpp"
#include "../common/ray.hpp"
#include "../common/vec3.hpp"

#include <cmath>
#include <vector>

struct Camera {
  double viewportWidth, viewportHeight;
  Point3 origin;

  Camera(const Point3 origin, const Point3 lookAt, const Vec3 up,
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
  Camera()
      : Camera(Point3(0, 0, 0),
               Point3(0, 0, -1),
               Vec3(0, 1, 0),
               60,
               1,
               0.1,
               1) {}

  [[nodiscard]] Ray getRay(const double x, const double y) const {
    Vec3 rd = Vec3::randomInUnitDisk() * lensRadius;
    Vec3 offset = u * rd.x + v * rd.y;
    return {origin + offset,
            topLeftCorner + horizontal * x + vertical * y - origin - offset};
  }

 private:
  Point3 topLeftCorner;
  Vec3 horizontal, vertical;
  Vec3 u, v, w;
  double lensRadius;
};

#endif //RAYTRACER_LIB_SCENE_CAMERA_HPP_
