//
// Created by Tyler Hostler-Mathis on 10/23/22.
//

#ifndef RAYTRACER_LIB_SCENE_CORNELL_BOX_HPP_
#define RAYTRACER_LIB_SCENE_CORNELL_BOX_HPP_

#include "../common/common.hpp"
#include "../common/vec3.hpp"
#include "../hittable/hittable.hpp"
#include "../hittable/material/lambertian.hpp"
#include "../hittable/material/emissive.hpp"
#include "../hittable/material/material.hpp"
#include "../hittable/objects/rectangle.hpp"

#include <memory>
#include <vector>

std::vector<std::shared_ptr<Hittable>> cornellBox() {
  std::vector<std::shared_ptr<Hittable>> scene;

  auto white = std::make_shared<Lambertian>(Color3(0.9, 0.9, 0.9));
  auto red = std::make_shared<Lambertian>(Color3(0.9, 0.1, 0.1));
  auto green = std::make_shared<Lambertian>(Color3(0.1, 0.1, 0.9));
  auto light = std::make_shared<Emissive>(Color3(1, 1, 1));

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

#endif //RAYTRACER_LIB_SCENE_CORNELL_BOX_HPP_
