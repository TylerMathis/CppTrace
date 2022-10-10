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

#include <memory>
#include <vector>

std::vector<std::shared_ptr<Hittable>> randomScene() {
  using common::randomDouble;
  using std::make_shared;

  std::vector<std::shared_ptr<Hittable>> hittables;

  auto groundMat = make_shared<Lambertian>(Color3(0.5, 0.5, 0.5));
  hittables.push_back(
      make_shared<Sphere>(Point3(0, -1000, 0), 1000, groundMat));

  Sphere deadzone(Point3(4, 0.2, 0), 0.9, groundMat);
  for (int a = -11; a <= 11; a++) {
    for (int b = -11; b <= 11; b++) {
      Point3 center(a + 0.9 * randomDouble(), 0.2, b + 0.9 * randomDouble());

      if ((center - deadzone.center).mag() > deadzone.radius) {
        std::shared_ptr<Material> sphereMat;

        double random = randomDouble();
        if (random < 0.8) {
          Color3 albedo(Vec3::random() * Vec3::random());
          sphereMat = make_shared<Lambertian>(albedo);
        } else if (random < 0.95) {
          Color3 albedo(Vec3::random(0.5, 1));
          double fuzz = randomDouble(0, 0.5);
          sphereMat = make_shared<Metal>(albedo, fuzz);
        } else {
          sphereMat = make_shared<Dielectric>(1.52);
        }

        hittables.push_back(make_shared<Sphere>(center, 0.2, sphereMat));
      }
    }
  }

  auto one = make_shared<Dielectric>(1.52);
  hittables.push_back(make_shared<Sphere>(Point3(0, 1, 0), 1.0, one));

  auto two = make_shared<Lambertian>(Color3(0.4, 0.2, 0.1));
  hittables.push_back(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, two));

  auto three = make_shared<Metal>(Color3(0.7, 0.6, 0.5), 0);
  hittables.push_back(make_shared<Sphere>(Point3(4, 1, 0), 1.0, three));

  return hittables;
}

int main() {
  const double aspectRatio = 3.0 / 2.0;
  const int width = 1200;
  const int height = width / aspectRatio;
  Image image("sphere.png", width, height, 3);

  auto hittables = randomScene();
  Scene scene(hittables);

  Point3 origin(13, 2, 3);
  Point3 lookAt(0, 0, 0);
  Point3 up(0, 1, 0);
  const double aperture = 0.1;
  const double focusDist = 10;
  const double fov = 20;
  const int samples = 10;
  const int depth = 50;
  Camera camera(origin, lookAt, up, fov, image.aspectRatio, aperture, focusDist,
                samples, depth);

  scene.render(camera, image);
}
