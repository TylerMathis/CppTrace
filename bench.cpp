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

#include <string>
#include <utility>
#include <memory>
#include <chrono>
#include <iostream>

struct Args {
  std::string modelpath, accelerator;
  Args(std::string modelpath, std::string accelerator)
      : modelpath(std::move(modelpath)), accelerator(std::move(accelerator)) {}
};

Args parse(int argc, char *argv[]) {
  cmdline::parser parser;

  parser.add<std::string>("modelpath", 'm', "Absolute path to model location", true);
  parser.add<std::string>("accelerator",
                          'a',
                          "Accelerator structure to benchmark",
                          true,
                          "",
                          cmdline::oneof<std::string>("bvh", "kdtree"));

  parser.parse_check(argc, argv);

  Args args(parser.get<std::string>("modelpath"), parser.get<std::string>("accelerator"));

  return args;
}

std::shared_ptr<Hittable> buildModel(std::string modelpath) {
  std::string extension = modelpath.substr(modelpath.find('.'));
  for (char &c : extension)
    if (std::isalpha(c))
      c = (char) std::tolower(c);

  std::shared_ptr<Texture> grey = std::make_shared<SolidColorTexture>(0.5, 0.5, 0.5);
  std::shared_ptr<Material> lambert = std::make_shared<Lambertian>(grey);

  if (extension == "obj") return std::make_shared<OBJ>(modelpath, lambert);
  else if (extension == "ply") return std::make_shared<PLY>(modelpath, lambert);
  else if (extension == "stl") return std::make_shared<STL>(modelpath, lambert);
  else
    throw std::invalid_argument("Invalid file extension '" + extension + "', only [.obj, .ply, .stl] are supported.");
}

int main(int argc, char *argv[]) {
  auto args = parse(argc, argv);

  auto object = buildModel(args.modelpath);

  const double aspectRatio = 1;
  const int width = 250;
  const int height = (int) (width / aspectRatio);
  const int samples = 1;
  const int bounces = 5;
  Image image(R"(D:\code\proj\CppTrace\images\booh.png)", width, height, samples, bounces);

  Point3 origin(0, 50, 100);
  Point3 lookAt(0, 50, 0);
  Point3 up(0, 1, 0);
  const double aperture = 0;
  const double focusDist = (origin - lookAt).mag();
  const double fov = 60;
  Camera camera(origin, lookAt, up, fov, image.aspectRatio, aperture,
                focusDist);

  Scene scene(std::make_shared<Camera>(camera),
              std::make_shared<Image>(image), "bvh");

  scene.pushHittables(object->getHittables());

  auto start = std::chrono::high_resolution_clock::now();
  scene.render(12);
  auto end = std::chrono::high_resolution_clock::now();

  auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << diff.count() << std::endl;

  return 0;
}
