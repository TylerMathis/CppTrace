#ifndef SCENE_H
#define SCENE_H

#include "./Hittable/Hittable.h"
#include "Camera.h"
#include "Hit.h"
#include "Image.h"
#include "Ray.h"
#include "Vec3.h"

#include <random>
#include <vector>

#include "time.h"

template <typename T> struct Scene {
  using Hittable = Hittable<T>;
  using Camera = Camera<T>;
  using Hit = Hit<T>;
  using Ray = Ray<T>;
  using Vec3 = Vec3<T>;
  using Color3 = Color3<T>;

  std::vector<Hittable *> hittables;

  Scene() { srand(time(NULL)); }

  void pushHittable(Hittable *h) { hittables.push_back(h); }
  void loadHittable(Hittable *h) { hittables = {h}; }
  void loadHittables(std::vector<Hittable *> hs) { hittables = hs; }

  Color3 getPixelColor(const Ray &r) const {
    Hit out, closest;
    bool found = false;
    for (Hittable *h : hittables)
      if (h->rayHit(r, out) && out < closest) {
        closest = out;
        found = true;
      }

    if (found)
      return (Color3(closest.normal) + 1) / 2;

    Vec3 unitDir = r.dir.unit();
    double t = (unitDir.y + 1.0) / 2;

    Color3 color = Color3(1, 1, 1) * (1.0 - t) + Color3(0.5, 0.7, 1.0) * t;
    return color;
  }

  void render(Camera &camera, Image &image) {

    for (int row = 0; row < image.height; row++) {
      for (int col = 0; col < image.width; col++) {
        Color3 aggregate(0, 0, 0);
        for (int sample = 0; sample < camera.samples; sample++) {
          double dx = (double)rand() / RAND_MAX;
          double dy = (double)rand() / RAND_MAX;
          double x = (col + dx) / image.width;
          double y = (row + dy) / image.height;
          Ray r = camera.getRay(x, y);
          aggregate += getPixelColor(r);
        }

        image.pushPixel((aggregate / camera.samples) * 255);
      }
    }

    image.write();
  }
};

#endif