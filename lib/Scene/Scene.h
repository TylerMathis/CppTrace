#ifndef SCENE_H
#define SCENE_H

#include "../Camera.h"
#include "../Common.h"
#include "../Hittable/Bounding/BVH.h"
#include "../Hittable/Hit.h"
#include "../Hittable/HittableList.h"
#include "../Image.h"
#include "../Ray.h"
#include "../Vec3.h"

#include <atomic>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <utility>
#include <vector>

template <typename T> struct _Scene {
  HittableList objects;
  BVH bvh;

  _Scene() {}
  _Scene(std::vector<std::shared_ptr<Hittable>> &hittables)
      : objects(hittables), bvh(objects) {}

  void pushHittable(std::shared_ptr<Hittable> hittable) {
    objects.pushHittable(hittable);
    bvh = BVH(objects);
  }
  void loadHittable(std::shared_ptr<Hittable> hittable) {
    objects.loadHittable(hittable);
    bvh = BVH(objects);
  }
  void loadHittables(std::vector<std::shared_ptr<Hittable>> hittables) {
    objects.loadHittables(hittables);
    bvh = BVH(objects);
  }

  Color3 getPixelColor(const Ray &ray, const int depth) const {
    if (depth <= 0)
      return Color3(0, 0, 0);

    Hit hit;
    if (bvh.hit(ray, hit)) {
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

  void render(Camera &camera, Image &image, const int threads = 7) {
    std::vector<std::vector<std::pair<int, int>>> locations(threads);
    int curThread = 0;
    for (int row = 0; row < image.height; row++)
      for (int col = 0; col < image.width; col++) {
        locations[curThread].emplace_back(row, col);
        curThread = (curThread + 1) % threads;
      }

    std::atomic<int> progress = 0;
    double entries = image.width * image.height;

    auto getJob = [&](int threadIdx) {
      for (auto [row, col] : locations[threadIdx]) {
        Color3 color(0, 0, 0);
        for (int sample = 0; sample < camera.samples; sample++) {
          double dx = common::randomDouble();
          double dy = common::randomDouble();
          double x = (col + dx) / image.width;
          double y = (row + dy) / image.height;
          Ray ray = camera.getRay(x, y);
          color += getPixelColor(ray, camera.bounceDepth);
        }

        image.setPixel(row, col, color, camera.samples);
        std::cerr << (++progress / entries) << "\n";
      }
    };

    std::vector<std::thread> jobs;
    for (int thread = 0; thread < threads; thread++)
      jobs.emplace_back(getJob, thread);

    for (auto &job : jobs)
      job.join();

    image.write();
  }
};

using Scene = _Scene<double>;

#endif
