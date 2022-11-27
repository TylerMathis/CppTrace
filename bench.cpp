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

struct Args {
  std::string modelpath, outputpath;

  Args(std::string modelpath, std::string outputpath)
      : modelpath(std::move(modelpath)), outputpath(std::move(outputpath)) {}
};

Args parse(int argc, char *argv[]) {
  cmdline::parser parser;

  parser.add<std::string>("modelpath", 'm', "Absolute path to model location", true);
  parser.add<std::string>("outputpath", 'o', "Absolute path of output png", true);

  parser.parse_check(argc, argv);

  Args args(parser.get<std::string>("modelpath"),
            parser.get<std::string>("outputpath"));

  return args;
}

std::shared_ptr<Hittable> buildModel(std::string modelpath) {
  std::string extension = modelpath.substr(modelpath.find('.'));
  for (char &c : extension)
    if (std::isalpha(c))
      c = (char) std::tolower(c);

  std::shared_ptr<Texture> grey = std::make_shared<SolidColorTexture>(0.5, 0.5, 0.5);
  std::shared_ptr<Material> lambert = std::make_shared<Lambertian>(grey);

  if (extension == ".obj") return std::make_shared<OBJ>(modelpath, lambert);
  else if (extension == ".ply") return std::make_shared<PLY>(modelpath, lambert);
  else if (extension == ".stl") return std::make_shared<STL>(modelpath, lambert);
  else
    throw std::invalid_argument("Invalid file extension '" + extension + "', only [.obj, .ply, .stl] are supported.");
}

std::string buildRenderPath(const std::string &outputpath, const ACCELERATOR &accelerator) {
  return outputpath + "_" + stringFromAccel(accelerator) + ".png";
}

int main(int argc, char *argv[]) {
  auto args = parse(argc, argv);

  auto object = buildModel(args.modelpath);
  const double aspectRatio = 1;
  const int width = 250;
  const int height = (int) (width / aspectRatio);
  const int samples = 1;
  const int bounces = 1;
  auto image = std::make_shared<Image>(args.outputpath, width, height, samples, bounces);

  Point3 origin(-3, 1, 0);
  Point3 lookAt(0, 1, 1);
  Point3 up(0, 1, 0);
  const double aperture = 0;
  const double focusDist = (origin - lookAt).mag();
  const double fov = 90;
  auto camera = std::make_shared<Camera>(origin, lookAt, up, fov, aspectRatio, aperture,
                                         focusDist);

  std::ofstream results(args.outputpath + ".csv");
  for (int acceleratorInt = BVH_ACCEL; acceleratorInt <= MADMAN_BVH; acceleratorInt++) {
    auto accelerator = static_cast<const ACCELERATOR>(acceleratorInt);
    results << stringFromAccel(accelerator);
    if (accelerator != MADMAN_BVH)
      results << ",";
  }
  results << "\n";

  for (int acceleratorInt = BVH_ACCEL; acceleratorInt <= MADMAN_BVH; acceleratorInt++) {
    auto accelerator = static_cast<const ACCELERATOR>(acceleratorInt);

    auto renderPath = buildRenderPath(args.outputpath, accelerator);
    image->reset(renderPath);
    Scene scene(camera, image, accelerator);

    if (accelerator == MADMAN_BVH) {
      scene.loadTriangles(object->getTriangles());
    } else {
      scene.loadHittables(object->getHittables());
    }
    scene.setAmbient(Color3(0.5, 0.5, 0.5));

    auto start = std::chrono::high_resolution_clock::now();
    scene.render(1);
    auto end = std::chrono::high_resolution_clock::now();

    auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    results << diff.count();
    if (accelerator != MADMAN_BVH)
      results << ",";
  }
  results << "\n";
  results.close();

  return 0;
}
