#include "./lib/Camera.h"
#include "./lib/Hittable/Material/Lambertian.h"
#include "./lib/Hittable/Material/Material.h"
#include "./lib/Hittable/Sphere.h"
#include "./lib/Image.h"
#include "./lib/Scene.h"
#include "./lib/Vec3.h"

int main() {
  Scene<double> scene;
  Lambertian<double> material =
      Lambertian<double>(Color3<double>(0.6, 0.7, 0.6));
  Sphere<double> c1(Point3<double>(0, 0, -1), 0.5, &material);
  Sphere<double> c2(Point3<double>(0, -100.5, -1), 100, &material);
  scene.loadHittables({&c1, &c2});

  Image image("sphere.png", 600, 300, 3);

  const double viewportHeight = 2;
  Camera<double> camera(viewportHeight * image.aspectRatio, viewportHeight, 1.0,
                        Point3<double>(0, 0, 0), 100, 1000);

  scene.render(camera, image);
}
