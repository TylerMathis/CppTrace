#include "./lib/Camera.h"
#include "./lib/Hittable/Hittable.h"
#include "./lib/Hittable/Material/Lambertian.h"
#include "./lib/Hittable/Material/Material.h"
#include "./lib/Hittable/Material/Metal.h"
#include "./lib/Hittable/Sphere.h"
#include "./lib/Image.h"
#include "./lib/Scene.h"
#include "./lib/Vec3.h"

#include <vector>

int main() {
  Metal leftMat = Metal(Color3(0.6, 0.7, 0.6));
  Metal rightMat = Metal(Color3(0.4, 0.5, 0.7));
  Lambertian middleMat = Lambertian(Color3(0.6, 0.7, 0.6));
  Lambertian groundMat = Lambertian(Color3(0.8, 0.3, 0.6));

  Sphere left(Point3(-1.1, 0, -1), 0.5, &leftMat);
  Sphere middle(Point3(0, 0, -1), 0.5, &middleMat);
  Sphere right(Point3(1.1, 0, -1), 0.5, &rightMat);
  Sphere ground(Point3(0, -100.5, -1), 100, &groundMat);

  std::vector<Hittable *> hittables = {&left, &middle, &right, &ground};
  Scene scene(hittables);

  Image image("sphere.png", 600, 300, 3);

  const double viewportHeight = 2;
  Camera camera(Point3(-2, 2, 1), Point3(0, 0, -1), Point3(0, 1, 0), 60,
                image.aspectRatio, 100, 1000);

  scene.render(camera, image);
}
