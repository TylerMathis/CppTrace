//
// Created by Tyler Hostler-Mathis on 10/18/22.
//

#ifndef CPPTRACE_LIB_SCENE_RANDOM_GEN_HPP_
#define CPPTRACE_LIB_SCENE_RANDOM_GEN_HPP_

#include "../common/common.hpp"
#include "../common/vec3.hpp"
#include "../hittable/hittable.hpp"
#include "../hittable/materials/dielectric.hpp"
#include "../hittable/materials/lambertian.hpp"
#include "../hittable/materials/emissive.hpp"
#include "../hittable/materials/material.hpp"
#include "../hittable/materials/metal.hpp"
#include "../hittable/objects/sphere.hpp"
#include "../hittable/textures/solid_color.hpp"

#include <ctime>
#include <iostream>
#include <memory>
#include <vector>

namespace random_scene {

const double refractionIndexGlass = 1.52;
std::vector<std::shared_ptr<Hittable>> randomScene(const int objects) {
  srand(time(NULL));
  const double locBound = 100;
  const double radBound = 4;
  const double colorBound = 1;
  std::vector<Sphere> scene;

  const auto invalidLoc = [&scene](const Vec3 &loc, const double rad) -> bool {
    return std::any_of(begin(scene),
                       end(scene),
                       [&loc, &rad](Sphere &s) {
                         return loc.dist(s.center) <= rad + s.radius;
                       });
  };

  for (int i = 1; i <= objects; i++) {
    std::cout << "Placing " << i << "th object.\n";
    double randomRad = std::sqrt(common::randomDouble(0.1, radBound));
    bool isBig = common::randomDouble() > 0.95;
    if (isBig)
      randomRad *= 3;
    Vec3 randomPos;
    do {
      randomPos = Vec3::random(-locBound, locBound);
      randomPos.y = randomRad;
    } while (invalidLoc(randomPos, randomRad));

    std::shared_ptr<Material> randomMat;
    Color3 randomColor = Color3::random(0.3, colorBound);
    auto texture = std::make_shared<SolidColorTexture>(randomColor);
    int materialType = common::randomInt(0, 4);
    switch (materialType) {
      default:
      case 0:randomMat = std::make_shared<Emissive>(texture);
        break;
      case 1:randomMat = std::make_shared<Metal>(texture);
        break;
      case 2:randomMat = std::make_shared<Lambertian>(texture);
        break;
      case 3:randomMat = std::make_shared<Dielectric>(refractionIndexGlass);
        break;
    }

    scene.emplace_back(randomPos, randomRad, randomMat);
  }

  std::vector<std::shared_ptr<Hittable>> ret;
  for (auto &s : scene)
    ret.push_back(std::make_shared<Sphere>(s));

  auto groundTexture =
      std::make_shared<SolidColorTexture>(Color3(0.5, 0.5, 0.5));
  auto ground = std::make_shared<Lambertian>(groundTexture);
  ret.push_back(
      std::make_shared<Sphere>(Point3(0, -100000, 0), 100000, ground));

  return ret;
}

} // namespace random_scene

#endif //CPPTRACE_LIB_SCENE_RANDOM_GEN_HPP_
