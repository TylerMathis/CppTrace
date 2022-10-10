#include "./lib/Camera.h"
#include "./lib/Hittable/Hittable.h"
#include "./lib/Hittable/Material/Dielectric.h"
#include "./lib/Hittable/Material/Lambertian.h"
#include "./lib/Hittable/Material/Material.h"
#include "./lib/Hittable/Material/Metal.h"
#include "./lib/Hittable/Sphere.h"
#include "./lib/Image.h"
#include "./lib/Scene.h"
#include "./lib/Vec3.h"

#include <memory>
#include <vector>

int main() {
  auto rightMat = std::make_shared<Metal>(Color3(0.4, 0.5, 0.7));
  auto middleMat = std::make_shared<Dielectric>(1.5);
  auto groundMat = std::make_shared<Lambertian>(Color3(0.8, 0.3, 0.6));
  auto smalls = std::make_shared<Lambertian>(Color3(0.5, 0.5, 0.5));

  auto middle = std::make_shared<Sphere>(Point3(0, 0, 0), 0.5, middleMat);
  auto right = std::make_shared<Sphere>(Point3(1.1, 0, 0), 0.5, rightMat);
  auto ground = std::make_shared<Sphere>(Point3(0, -100.5, 0), 100, groundMat);
  auto one = std::make_shared<Sphere>(Point3(0, -0.35, 0.6), 0.15, smalls);

  std::vector<std::shared_ptr<Hittable>> hittables = {middle, right, ground,
                                                      one};
  Scene scene(hittables);

  Image image("sphere.png", 1000, 1000, 3);

  Point3 origin(-2, 0.5, 1.5);
  Point3 lookAt(0, 0, 0);
  Point3 up(0, 1, 0);
  double aperture = 0.1;
  double focusDist = (origin - lookAt).mag();
  double fov = 45;
  int samples = 1;
  int bounces = 50;
  Camera camera(origin, lookAt, up, fov, image.aspectRatio, aperture, focusDist,
                samples, bounces);

  scene.render(camera, image);
}
