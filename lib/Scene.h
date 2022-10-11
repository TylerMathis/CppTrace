#ifndef SCENE_H
#define SCENE_H

#include "./Hittable/Hit.h"
#include "./Hittable/HittableList.h"
#include "Camera.h"
#include "Image.h"
#include "Ray.h"
#include "Vec3.h"

#include <iostream>
#include <random>
#include <string>

template <typename T> struct _Scene {
  HittableList hittables;

  _Scene() {}
  _Scene(std::vector<std::shared_ptr<Hittable>> &hittables)
      : hittables(hittables) {}

  void pushHittable(std::shared_ptr<Hittable> hittable) {
    hittables.pushHittable(hittable);
  }
  void loadHittable(std::shared_ptr<Hittable> hittable) {
    hittables.loadHittable(hittable);
  }
  void loadHittables(std::vector<std::shared_ptr<Hittable>> _hittables) {
    hittables.loadHittable(_hittables);
  }

  Color3 getPixelColor(const Ray &ray, const int depth) const {
    if (depth <= 0)
      return Color3(0, 0, 0);

    Hit hit;
    if (hittables.hit(ray, hit)) {
      Color3 attenuation;
      Ray out;
      hit.material->scatter(ray, hit, attenuation, out);
      return getPixelColor(out, depth - 1) * attenuation;
    }

    Vec3 unitDirection = ray.direction.unit();
    double t = (unitDirection.y + 1.0) / 2;

    Color3 color = Color3(1, 1, 1) * (1.0 - t) + Color3(0.5, 0.7, 1.0) * t;
    return color;
  }

  void render(Camera &camera, Image &image) {
    for (int row = 0; row < image.height; row++) {
      std::cerr << "Processing scanline " << row << std::endl;
      for (int col = 0; col < image.width; col++) {

        Color3 color(0, 0, 0);
        for (int sample = 0; sample < camera.samples; sample++) {
          double dx = (double)rand() / RAND_MAX;
          double dy = (double)rand() / RAND_MAX;
          double x = (col + dx) / image.width;
          double y = (row + dy) / image.height;
          Ray ray = camera.getRay(x, y);
          color += getPixelColor(ray, camera.bounceDepth);
        }

        image.pushPixel(color, camera.samples);
      }
    }

    image.write();
  }
};

using Scene = _Scene<double>;

#endif
