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

#include <memory>
#include <vector>

int main() {
  srand(time(NULL));

  const double aspectRatio = 16.0 / 9.0;
  const int width = 1200;
  const int height = width / aspectRatio;
  Image image("lowqual", width, height, 3);

  Point3 origin(1, 0.5, 3);
  Point3 lookAt(0.5, 0.5, 0);
  Point3 up(0, 1, 0);
  const double aperture = 0.1;
  const double focusDist = (origin - lookAt).mag();
  const double fov = 90;
  const int samples = 250;
  const int depth = 50;
  Camera camera(origin, lookAt, up, fov, image.aspectRatio, aperture, focusDist,
                samples, depth);

  auto groundMat = std::make_shared<Lambertian>(Color3(0.07, 0.07, 0.07));
  auto glassMat = std::make_shared<Dielectric>(1.52);
  auto diffuseMat = std::make_shared<Lambertian>(Color3(0.8, 0.4, 0.7));
  auto metalMat = std::make_shared<Metal>(Color3(0.4, 0.9, 0.3));
  auto behindMat = std::make_shared<Lambertian>(Color3(0.6, 0.4, 0.9));

  auto ground = std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, groundMat);
  auto glass = std::make_shared<Sphere>(Point3(0, 1, 0), 1, glassMat);
  auto metal = std::make_shared<Sphere>(Point3(2, 0.5, 0), 0.5, metalMat);
  auto ball = std::make_shared<Sphere>(Point3(1, 0.3, 0.45), 0.3, behindMat);
  auto behind = std::make_shared<Sphere>(Point3(0.5, 0.3, -5), 0.3, behindMat);
  auto another =
      std::make_shared<Sphere>(Point3(-0.5, 0.3, -4.5), 0.3, diffuseMat);
  std::vector<std::shared_ptr<Hittable>> hittables = {ground, glass,  metal,
                                                      ball,   behind, another};

  Scene scene(hittables);
  scene.render(camera, image);
}
