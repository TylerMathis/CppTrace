#ifndef SCENE_H
#define SCENE_H

#include "./Hittable/Hit.h"
#include "./Hittable/Hittable.h"
#include "Camera.h"
#include "Image.h"
#include "Ray.h"
#include "Vec3.h"

#include <iostream>
#include <memory>
#include <random>
#include <vector>

template <typename T> struct _Scene {
  std::vector<std::shared_ptr<Hittable>> hittables;

  _Scene() {}
  _Scene(std::vector<std::shared_ptr<Hittable>> &hittables)
      : hittables(hittables) {}

  void pushHittable(std::shared_ptr<Hittable> hittable) {
    hittables.push_back(hittable);
  }
  void loadHittable(std::shared_ptr<Hittable> hittable) {
    hittables = {hittable};
  }
  void loadHittables(std::vector<std::shared_ptr<Hittable>> _hittables) {
    hittables = _hittables;
  }

  Color3 getPixelColor(const Ray &ray, const int depth) const {
    if (depth <= 0)
      return Color3(0, 0, 0);

    Hit out, closest;
    bool found = false;
    for (auto hittable : hittables)
      if (hittable->rayCast(ray, out) && out < closest) {
        closest = out;
        found = true;
      }

    if (found) {
      Color3 attenuation;
      Ray out;
      closest.material->scatter(ray, closest, attenuation, out);
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

    std::cerr << "Done, writing image\n";
    image.write();
  }
};

using Scene = _Scene<double>;

#endif
