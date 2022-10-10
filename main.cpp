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

#include <vector>

int main() {
  Metal rightMat = Metal(Color3(0.4, 0.5, 0.7));
  Dielectric middleMat = Dielectric(1.5);
  Lambertian groundMat = Lambertian(Color3(0.8, 0.3, 0.6));
  Lambertian smalls = Lambertian(Color3(0.5, 0.5, 0.5));

  Sphere middle(Point3(0, 0, 0), 0.5, &middleMat);
  Sphere right(Point3(1.1, 0, 0), 0.5, &rightMat);
  Sphere ground(Point3(0, -100.5, 0), 100, &groundMat);
  Sphere one(Point3(0, -0.35, 0.6), 0.15, &smalls);

  std::vector<Hittable *> hittables = {&middle, &right, &ground, &one};
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
