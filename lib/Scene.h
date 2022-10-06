#ifndef SCENE_H
#define SCENE_H

#include "./Primitive/Primitive.h"
#include "Camera.h"
#include "Hit.h"
#include "Image.h"
#include "Ray.h"
#include "Vec3.h"

#include <vector>

template <typename T> struct Scene {
  using Primitive = Primitive<T>;
  using Camera = Camera<T>;
  using Hit = Hit<T>;
  using Ray = Ray<T>;
  using Vec3 = Vec3<T>;

  std::vector<Primitive *> primitives;

  Scene() {}

  void pushPrimitive(Primitive *p) { primitives.push_back(p); }
  void loadPrimitive(Primitive *p) { primitives = {p}; }
  void loadPrimitives(std::vector<Primitive *> ps) { primitives = ps; }

  Color3<uint8_t> getPixelColor(const Ray &r) const {
    Hit out, closest;
    bool found = false;
    for (Primitive *p : primitives)
      if (p->rayHit(r, out) && out.dist < closest.dist) {
        closest = out;
        found = true;
      }

    if (found)
      return ((Color3<>(closest.normal) + 1) / 2) * 255;

    Vec3 unitDir = r.dir.unit();
    double t = (unitDir.y + 1.0) / 2;

    Color3<> exactColor =
        Color3<>(1, 1, 1) * (1.0 - t) + Color3<>(0.5, 0.7, 1.0) * t;
    Color3<uint8_t> pixelColor(exactColor * 255);

    return pixelColor;
  }

  void render(Camera &camera, Image &image) {
    for (int row = 0; row < image.height; row++) {
      for (int col = 0; col < image.width; col++) {
        double x = (double)col / image.width;
        double y = (double)row / image.height;

        Ray r = camera.getRay(x, y);
        Color3<uint8_t> color = getPixelColor(r);
        image.pushPixel(color);
      }
    }

    image.write();
  }
};

#endif