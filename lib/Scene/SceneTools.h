#ifndef SCENE_TOOLS_H
#define SCENE_TOOLS_H

#include "../Camera.h"
#include "../Common.h"
#include "../Hittable/Hittable.h"
#include "../Hittable/Material/Dielectric.h"
#include "../Hittable/Material/Lambertian.h"
#include "../Hittable/Material/Material.h"
#include "../Hittable/Material/Metal.h"
#include "../Hittable/Sphere.h"
#include "../Image.h"
#include "../Vec3.h"
#include "Scene.h"

#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

using std::cerr;
using std::ifstream;
using std::make_shared;
using std::shared_ptr;
using std::string;
using std::stringstream;
using std::vector;

struct SceneTools {
  static bool interpretScene(const string scenePath, Scene &scene) {
    ifstream sceneStream(scenePath);

    if (!sceneStream) {
      cerr << "Invalid scene path\n";
      return false;
    }

    Camera *camera = nullptr;
    Image *image = nullptr;
    vector<shared_ptr<Hittable>> objects;

    int curLine = 0;
    string line;
    while (std::getline(sceneStream, line)) {
      curLine++;

      stringstream lineStream(line);
      vector<string> tokens;
      string token;
      while (lineStream >> token)
        tokens.push_back(token);

      // Empty line
      if (tokens.size() == 0)
        continue;

      token = tokens[0];
      if (token == "Camera") {
        if (camera != nullptr) {
          cerr << curLine << ": Scenes must only have one Camera\n";
          return false;
        }
        if (!parseCamera(sceneStream, camera))
          return false;
      } else if (token == "Image") {
        if (image != nullptr) {
          cerr << curLine << ": Scenes must only have one Image\n";
          return false;
        }
        if (!parseImage(sceneStream, image))
          return false;
      } else if (token == "Sphere") {
        Sphere sphere;
        if (!parseSphere(sceneStream, sphere))
          return false;
        objects.push_back(make_shared<Sphere>(sphere));
      }
    }

    scene = Scene(*camera, *image, objects);
    return true;
  }

private:
  static bool parseCamera(const ifstream &sceneStream, Camera *camera);
  static bool parseImage(const ifstream &sceneStream, Image *image);
  static bool parseSphere(const ifstream &sceneStream, Sphere &spher);
};

#endif