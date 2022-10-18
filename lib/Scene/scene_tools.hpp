//
// Created by Tyler Hostler-Mathis on 10/18/22.
//

#ifndef RAYTRACER_LIB_SCENE_SCENE_TOOLS_HPP_
#define RAYTRACER_LIB_SCENE_SCENE_TOOLS_HPP_

#include "camera.hpp"
#include "image.hpp"
#include "scene.hpp"
#include "../cli/progress_indicator.hpp"
#include "../common/common.hpp"
#include "../common/ray.hpp"
#include "../common/vec3.hpp"
#include "../hittable/Bounding/BVH.hpp"
#include "../hittable/hit.hpp"
#include "../hittable/hittable_list.hpp"
#include "../hittable/sphere.hpp"

#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

struct SceneTools {
  static bool interpretScene(const std::string &scenePath, Scene &scene) {
    std::ifstream sceneStream(scenePath);

    if (!sceneStream) {
      std::cerr << "Invalid scene path\n";
      return false;
    }

    std::shared_ptr<Camera> camera;
    std::shared_ptr<Image> image;
    std::vector<std::shared_ptr<Hittable>> objects;

    int curLine = 0;
    std::string line;
    while (std::getline(sceneStream, line)) {
      curLine++;

      std::stringstream lineStream(line);
      std::vector<std::string> tokens;
      std::string token;
      while (lineStream >> token)
        tokens.push_back(token);

      // Empty line
      if (tokens.empty())
        continue;

      token = tokens[0];
      if (token == "Camera") {
        if (camera != nullptr) {
          std::cerr << curLine << ": Scenes must only have one Camera\n";
          return false;
        }
        if (!parseCamera(sceneStream, camera))
          return false;
      } else if (token == "Image") {
        if (image != nullptr) {
          std::cerr << curLine << ": Scenes must only have one Image\n";
          return false;
        }
        if (!parseImage(sceneStream, image))
          return false;
      } else if (token == "Sphere") {
        std::shared_ptr<Sphere> sphere;
        if (!parseSphere(sceneStream, sphere))
          return false;
        objects.push_back(sphere);
      }
    }

    scene = Scene(camera, image, objects);
    return true;
  }

 private:
  static bool parseCamera(const std::ifstream &sceneStream,
                          const std::shared_ptr<Camera> &camera);
  static bool parseImage(const std::ifstream &sceneStream,
                         const std::shared_ptr<Image> &image);
  static bool parseSphere(const std::ifstream &sceneStream,
                          const std::shared_ptr<Sphere> &sphere);
};

#endif //RAYTRACER_LIB_SCENE_SCENE_TOOLS_HPP_
