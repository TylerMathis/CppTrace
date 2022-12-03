//
// Created by Tyler Hostler-Mathis on 11/7/22.
//

#include "./lib/extern/cmdline.hpp"
#include "./lib/hittable/hittable.hpp"
#include "./lib/hittable/objects/obj.hpp"
#include "./lib/hittable/objects/ply.hpp"
#include "./lib/hittable/objects/stl.hpp"
#include "./lib/hittable/textures/solid_color.hpp"
#include "./lib/scene/camera.hpp"
#include "./lib/scene/scene.hpp"
#include "./lib/accelerators/accelerators.hpp"

#include <string>
#include <utility>
#include <memory>
#include <chrono>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

std::vector<std::string> models = {"booh", "pickle"};

std::map<std::string, Point3> origins = {
    {"booh", Point3(0, 40, 75)},
    {"pickle", Point3(20, 12.5, 20)},
};

std::map<std::string, double> heights = {
    {"booh", 100},
    {"pickle", 150},
};

struct Args {
  std::string modelpath, outputpath;

  Args(std::string modelpath, std::string outputpath)
      : modelpath(std::move(modelpath)), outputpath(std::move(outputpath)) {}
};

Args parse(int argc, char *argv[]) {
  cmdline::parser parser;

  parser.add<std::string>("modelpath", 'm', "Absolute path to model folder", true);
  parser.add<std::string>("outputpath", 'o', "Absolute path of output folder", true);

  parser.parse_check(argc, argv);

  Args args(parser.get<std::string>("modelpath"),
            parser.get<std::string>("outputpath"));

  return args;
}

std::shared_ptr<STL> buildModel(std::string modelpath) {
  std::shared_ptr<Texture> grey = std::make_shared<SolidColorTexture>(0.5, 0.5, 0.5);
  std::shared_ptr<Material> lambert = std::make_shared<Lambertian>(grey);
  return std::make_shared<STL>(modelpath, lambert);
}

std::string buildRenderPath(const std::string &outputpath, const ACCELERATOR &accelerator) {
  return outputpath + "_" + stringFromAccel(accelerator) + ".png";
}

int main(int argc, char *argv[]) {
  auto args = parse(argc, argv);

  std::ofstream results(args.outputpath + "results.csv");
  results << "triangles,accelerator,build,render\n";

  for (const auto& model : models) {
    auto object = buildModel(args.modelpath + model + ".stl");

    const int width = 100;
    const int height = heights[model];
    const int samples = 1;
    const int bounces = 1;
    auto image = std::make_shared<Image>(args.outputpath + model + ".png", width, height, samples, bounces);

    auto origin = origins[model];
    Point3 lookAt(0, origin.y, 0);
    Point3 up(0, 1, 0);
    const double aperture = 0;
    const double focusDist = (origin - lookAt).mag();
    const double fov = 60;
    auto camera = std::make_shared<Camera>(origin, lookAt, up, fov, image->aspectRatio, aperture,
                                           focusDist);

    for (int acceleratorInt = KD_TREE_ACCEL; acceleratorInt <= FAST_BVH; acceleratorInt++) {
      auto accelerator = static_cast<const ACCELERATOR>(acceleratorInt);
      results << object->numTriangles << "," << stringFromAccel(accelerator) << ",";

      auto renderPath = buildRenderPath(args.outputpath + model, accelerator);
      image->reset(renderPath);
      Scene scene(camera, image, accelerator);

      auto start = std::chrono::high_resolution_clock::now();
      if (accelerator == MADMAN_BVH || accelerator == FAST_BVH) {
        scene.loadTriangles(object->getTriangles());
      } else {
        scene.loadHittables(object->getHittables());
      }
      auto end = std::chrono::high_resolution_clock::now();
      auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
      results << diff.count() << ",";

      scene.setAmbient(Color3(0.5, 0.5, 0.5));

      start = std::chrono::high_resolution_clock::now();
      scene.render(1);
      end = std::chrono::high_resolution_clock::now();

      diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
      results << diff.count() << "\n";
    }
  }

  results.close();
  return 0;
}
