#include "./lib/scene/camera.hpp"
#include "./lib/hittable/material/dielectric.hpp"
#include "./lib/hittable/material/lambertian.hpp"
#include "./lib/hittable/material/metal.hpp"
#include "./lib/hittable/obj.hpp"
#include "./lib/hittable/sphere.hpp"
#include "./lib/scene/image.hpp"
#include "./lib/scene/scene.hpp"

#include <memory>

int main() {
  const double aspectRatio = 16.0 / 9.0;
  const int width = 800;
  const int height = (int) (width / aspectRatio);
  const int samples = 10;
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

  std::string objPath = "objects/horse.obj";
  auto material = std::make_shared<Dielectric>(1.52);
  auto object = std::make_shared<OBJ>(objPath, material);
  auto sphereMat = std::make_shared<Lambertian>(Color3(0.7, 0.5, 0.6));
  auto sphere = std::make_shared<Sphere>(Point3(-3, 1, 0), 0.75, sphereMat);
  auto sphereMat2 = std::make_shared<Metal>(Color3(0.7, 0.5, 0.6));
  auto sphere2 = std::make_shared<Sphere>(Point3(-3, 1, 3), 0.75, sphereMat2);
  Scene scene(std::make_shared<Camera>(camera),
              std::make_shared<Image>(image),
              {object, sphere, sphere2});
  scene.render();
}