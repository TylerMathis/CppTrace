//
// Created by Tyler Hostler-Mathis on 10/18/22.
//

#ifndef CPPTRACE_LIB_SCENE_SCENE_HPP_
#define CPPTRACE_LIB_SCENE_SCENE_HPP_

#include "camera.hpp"
#include "image.hpp"
#include "../accelerators/accelerators.hpp"
#include "../cli/progress_indicator.hpp"
#include "../common/common.hpp"
#include "../common/ray.hpp"
#include "../common/vec3.hpp"
#include "../accelerators/bvh.hpp"
#include "../accelerators/madman_bvh.hpp"
#include "../accelerators/kd_tree.hpp"
#include "../hittable/hit.hpp"
#include "../hittable/hittable_list.hpp"

#include "unistd.h"

#include <atomic>
#include <chrono>
#include <iostream>
#include <random>
#include <thread>
#include <utility>
#include <vector>
#include <memory>

struct Scene {
  std::shared_ptr<Camera> camera;
  std::shared_ptr<Image> image;

  Color3 ambient;

  ACCELERATOR acceleratorType;
  HittableList objects;
  std::shared_ptr<Hittable> accelerator;

  explicit Scene(std::shared_ptr<Camera> camera,
                 std::shared_ptr<Image> image,
                 const ACCELERATOR acceleratorType,
                 const Color3 &ambient = {0, 0, 0})
      : camera(std::move(camera)),
        image(std::move(image)),
        ambient(ambient),
        acceleratorType(acceleratorType) {}

  void notifyAccelerator() {
    if (acceleratorType == BVH_ACCEL) {
      accelerator = std::make_shared<BVH>(objects);
    } else if (acceleratorType == MADMAN_BVH) {
      accelerator = std::make_shared<MadmanBVH>(objects.triangles, objects.triangles[0]->material);
    } else if (acceleratorType == KD_TREE) {
      accelerator = std::make_shared<KDTree>(objects);
    }
  }

  void setAmbient(const Color3 &_ambient) {
    ambient = _ambient;
  }

  void pushHittable(const std::shared_ptr<Hittable> &hittable) {
    objects.pushHittable(hittable);
    notifyAccelerator();
  }
  void pushHittables(const std::vector<std::shared_ptr<Hittable>> &hittables) {
    for (auto &hittable : hittables)
      objects.pushHittable(hittable);
    notifyAccelerator();
  }
  void loadHittable(const std::shared_ptr<Hittable> &hittable) {
    objects.loadHittable(hittable);
    notifyAccelerator();
  }
  void loadHittables(const std::vector<std::shared_ptr<Hittable>> &hittables) {
    objects.loadHittables(hittables);
    notifyAccelerator();
  }
  void loadTriangles(const std::vector<std::shared_ptr<Triangle>> &triangles) {
    objects.loadTriangles(triangles);
    notifyAccelerator();
  }

  // Recursively scatter the ray, depth limited by bouncesLeft
  [[nodiscard]] Color3 getPixelColor(const Ray &ray,
                                     const int bouncesLeft) const {
    if (bouncesLeft <= 0)
      return {0, 0, 0};

    Hit hit = accelerator->hit(ray, 0.001, 10000000);
    if (!hit.valid) {
      return ambient;
    }

    Color3 attenuation;
    Ray out;
    Color3 emitted = hit.material->emit(hit.u, hit.v, hit.location);
    if (!hit.material->scatter(ray, hit, attenuation, out)) {
      return emitted;
    }

    return emitted + getPixelColor(out, bouncesLeft - 1) * attenuation;
  }

  void render(const int threads = 4) const {
    std::cout << "Beginning render\n";
    std::vector<std::vector<std::pair<int, int>>> locations(threads);
    int curThread = 0;
    for (int row = 0; row < image->height; row++)
      for (int col = 0; col < image->width; col++) {
        locations[curThread].emplace_back(row, col);
        curThread = (curThread + 1) % threads;
      }

    std::atomic<int> progress = 0;
    int entries = image->width * image->height;
    ProgressIndicator progressIndicator(entries);

    auto getJob = [&](int threadIdx) {
      for (auto [row, col] : locations[threadIdx]) {
        Color3 color(0, 0, 0);
        for (int sample = 0; sample < image->samples; sample++) {
          double dx = common::randomDouble();
          double dy = common::randomDouble();
          double x = (col + dx) / image->width;
          double y = (row + dy) / image->height;
          Ray ray = camera->getRay(x, y);
          color += getPixelColor(ray, image->bounces);
        }

        image->setPixel(row, col, color);
        progress++;
      }
    };

    std::vector<std::thread> jobs;
    for (int thread = 0; thread < threads; thread++)
      jobs.emplace_back(getJob, thread);

    while (progress < entries) {
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
      progressIndicator.indicate(progress);
    }
    progressIndicator.done();

    for (auto &job : jobs)
      job.join();

    image->write();
  }
};

#endif //CPPTRACE_LIB_SCENE_SCENE_HPP_
