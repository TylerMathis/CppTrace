//
// Created by Tyler Hostler-Mathis on 10/23/22.
//

#ifndef CPPTRACE_LIB_SCENE_CORNELL_BOX_HPP_
#define CPPTRACE_LIB_SCENE_CORNELL_BOX_HPP_

#include "../common/common.hpp"
#include "../common/vec3.hpp"
#include "../hittable/hittable.hpp"
#include "../hittable/materials/lambertian.hpp"
#include "../hittable/materials/emissive.hpp"
#include "../hittable/materials/material.hpp"
#include "../hittable/objects/rectangle.hpp"
#include "../hittable/objects/rectangular_prism.hpp"
#include "../hittable/objects/sphere.hpp"
#include "../hittable/textures/solid_color.hpp"
#include "../hittable/textures/image.hpp"
#include "../hittable/objects/constant_medium.hpp"

#include <memory>
#include <vector>

namespace cornell_scene {

std::vector<std::shared_ptr<Hittable>> cornellBox() {
  std::vector<std::shared_ptr<Hittable>> scene;

  auto white = std::make_shared<Lambertian>(std::make_shared<SolidColorTexture>(
      0.9,
      0.9,
      0.9));
  auto red = std::make_shared<Lambertian>(std::make_shared<SolidColorTexture>(
      0.9,
      0.1,
      0.1));
  auto green = std::make_shared<Lambertian>(std::make_shared<SolidColorTexture>(
      0.1,
      0.1,
      0.9));
  auto light =
      std::make_shared<Emissive>(std::make_shared<SolidColorTexture>(Color3(1,
                                                                            1,
                                                                            1)));

  auto floorTri =
      Triangle(Point3(-1, 0, 1), Point3(-1, 0, -1), Point3(1, 0, -1), white);
  auto floor = std::make_shared<Rectangle>(floorTri);
  scene.push_back(floor);

  auto leftWallTri =
      Triangle(Point3(-1, 0, 1), Point3(-1, 0, -1), Point3(-1, 2, -1), red);
  auto leftWall = std::make_shared<Rectangle>(leftWallTri);
  scene.push_back(leftWall);

  auto rightWallTri =
      Triangle(Point3(1, 0, 1), Point3(1, 0, -1), Point3(1, 2, -1), green);
  auto rightWall = std::make_shared<Rectangle>(rightWallTri);
  scene.push_back(rightWall);

  auto backWallTri =
      Triangle(Point3(-1, 0, -1), Point3(1, 0, -1), Point3(1, 2, -1), white);
  auto backWall = std::make_shared<Rectangle>(backWallTri);
  scene.push_back(backWall);

  auto topWallTri =
      Triangle(Point3(-1, 2, 1), Point3(-1, 2, -1), Point3(1, 2, -1), white);
  auto topWall = std::make_shared<Rectangle>(topWallTri);
  scene.push_back(topWall);

  auto topLightTri =
      Triangle(Point3(-0.25, 1.99, 0.25),
               Point3(-0.25, 1.99, -0.25),
               Point3(0.25, 1.99, -0.25),
               light);
  auto topLight = std::make_shared<Rectangle>(topLightTri);
  scene.push_back(topLight);

  return scene;
}

std::vector<std::shared_ptr<Hittable>> earth() {
  std::vector<std::shared_ptr<Hittable>> scene;

  auto earthTexture = std::make_shared<ImageTexture>("textures/earthmap.jpeg");
  auto earthMat = std::make_shared<Lambertian>(earthTexture);
  auto earth = std::make_shared<Sphere>(Point3(0, 1, 0), 0.5, earthMat);
  scene.push_back(earth);

  return scene;
}

std::vector<std::shared_ptr<Hittable>> prism() {
  std::vector<std::shared_ptr<Hittable>> scene;

  auto prismTexture = std::make_shared<SolidColorTexture>(0.5, 0.5, 0.5);
  auto prismMat = std::make_shared<Lambertian>(prismTexture);
  auto prism = std::make_shared<RectangularPrism>(Point3(-0.5, 0.01, -0.5),
                                                  Point3(0.5, 1.2, 0.5),
                                                  prismMat);

  scene.push_back(std::make_shared<ConstantMedium>(prism,
                                                   0.5,
                                                   Color3(0, 0, 0)));

  return scene;
}

} // namespace cornell


#endif //CPPTRACE_LIB_SCENE_CORNELL_BOX_HPP_
