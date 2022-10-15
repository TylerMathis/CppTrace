#include "./lib/Camera.h"
#include "./lib/Hittable/Hittable.h"
#include "./lib/Hittable/Material/Dielectric.h"
#include "./lib/Hittable/Material/Lambertian.h"
#include "./lib/Hittable/Sphere.h"
#include "./lib/Hittable/Triangle.h"
#include "./lib/Image.h"
#include "./lib/Scene/Scene.h"
#include "./lib/Vec3.h"

#include <memory>
#include <vector>

int main() {
  const double aspectRatio = 16.0 / 9.0;
  const int width = 1200;
  const int height = width / aspectRatio;
  const int samples = 50;
  const int bounces = 50;
  Image image("triangles", width, height, samples, bounces);

  Point3 origin(0, 0, 1);
  Point3 lookAt(0, 0, 0);
  Point3 up(0, 1, 0);
  const double aperture = 0.01;
  const double focusDist = (origin - lookAt).mag();
  const double fov = 60;
  Camera camera(origin, lookAt, up, fov, image.aspectRatio, aperture,
                focusDist);

  std::vector<std::shared_ptr<Hittable>> world;

  auto triMat = std::make_shared<Dielectric>(1.52);
  auto a = Point3(-1, 0, 0), b = Point3(1, 0.2, 0), c = Point3(1, -0.2, 0);
  auto tri = std::make_shared<Triangle>(a, b, c, triMat);

  auto sphereMat = std::make_shared<Lambertian>(Color3(0.7, 0.6, 0.8));
  auto sphere = std::make_shared<Sphere>(Point3(0, 0, -1.2), 0.5, sphereMat);

  Scene scene;
  scene.loadHittables({tri, sphere});
  scene.render(camera, image);
}
