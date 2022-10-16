#include "./lib/Camera.h"
#include "./lib/Hittable/Material/Dielectric.h"
#include "./lib/Hittable/Material/Lambertian.h"
#include "./lib/Hittable/Material/Metal.h"
#include "./lib/Hittable/OBJ.h"
#include "./lib/Hittable/Sphere.h"
#include "./lib/Image.h"
#include "./lib/Scene/Scene.h"

#include <memory>

int main() {
  const double aspectRatio = 16.0 / 9.0;
  const int width = 1000;
  const int height = (int) (width / aspectRatio);
  const int samples = 100;
  const int bounces = 50;
  Image image("horse", width, height, samples, bounces);

  Point3 origin(3, 1, 0);
  Point3 lookAt(0, 1, 0);
  Point3 up(0, 1, 0);
  const double aperture = 0.01;
  const double focusDist = (origin - lookAt).mag();
  const double fov = 60;
  Camera camera(origin, lookAt, up, fov, image.aspectRatio, aperture,
                focusDist);

  Scene scene;
  std::string objPath = "./objects/horse.obj";
  auto material = std::make_shared<Dielectric>(1.52);
  auto object = std::make_shared<OBJ>(objPath, material);
  auto sphereMat = std::make_shared<Lambertian>(Color3(0.7, 0.5, 0.6));
  auto sphere = std::make_shared<Sphere>(Point3(-3, 1, 0), 0.75, sphereMat);
  auto sphereMat2 = std::make_shared<Metal>(Color3(0.7, 0.5, 0.6));
  auto sphere2 = std::make_shared<Sphere>(Point3(-3, 1, 3), 0.75, sphereMat2);
  scene.loadHittables({object, sphere, sphere2});
  scene.render(camera, image);
}
