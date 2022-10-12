#include "./lib/Camera.h"
#include "./lib/Common.h"
#include "./lib/Hittable/Hittable.h"
#include "./lib/Hittable/Material/Dielectric.h"
#include "./lib/Hittable/Material/Lambertian.h"
#include "./lib/Hittable/Material/Material.h"
#include "./lib/Hittable/Material/Metal.h"
#include "./lib/Hittable/Sphere.h"
#include "./lib/Image.h"
#include "./lib/Scene.h"
#include "./lib/Vec3.h"

#include "time.h"

#include <iostream>
#include <memory>
#include <vector>

const double refractionIndexGlass = 1.52;
std::vector<std::shared_ptr<Hittable>> randomScene(const int objects) {
  const double locBound = 400;
  const double radBound = 4;
  const double colorBound = 1;
  std::vector<Sphere> scene;

  const auto invalidLoc = [&scene](const Vec3 &loc, const double rad) -> bool {
    for (auto &s : scene)
      if (loc.dist(s.center) <= rad + s.radius)
        return true;
    return false;
  };

  for (int i = 1; i <= objects; i++) {
    std::cerr << "Placing " << i << "th object.\n";
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
    Color3 randomColor = Color3::random(0.2, colorBound);
    int materialType = common::randomInt(0, 4);
    if (materialType <= 2)
      randomMat = std::make_shared<Lambertian>(randomColor);
    else if (materialType <= 3)
      randomMat = std::make_shared<Metal>(randomColor);
    else
      randomMat = std::make_shared<Dielectric>(refractionIndexGlass);

    scene.push_back(Sphere(randomPos, randomRad, randomMat));
  }

  std::vector<std::shared_ptr<Hittable>> ret;
  for (auto &s : scene)
    ret.push_back(std::make_shared<Sphere>(s));

  auto ground = std::make_shared<Lambertian>(Color3(0.8, 0.5, 0.7));
  ret.push_back(
      std::make_shared<Sphere>(Point3(0, -100000, 0), 100000, ground));

  return ret;
}

int main() {
  srand(78493274);

  const double aspectRatio = 16.0 / 9.0;
  const int width = 1200;
  const int height = width / aspectRatio;
  Image image("manyObjects", width, height, 3);

  Point3 origin(0, 8, 20);
  Point3 lookAt(0, 0, 0);
  Point3 up(0, 1, 0);
  const double aperture = 0.2;
  const double focusDist = (origin - lookAt).mag();
  const double fov = 60;
  const int samples = 250;
  const int depth = 50;
  Camera camera(origin, lookAt, up, fov, image.aspectRatio, aperture, focusDist,
                samples, depth);

  auto world = randomScene(10000);
  Scene scene(world);
  scene.render(camera, image);
}
