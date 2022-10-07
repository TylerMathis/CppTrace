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
  using Lambertian = Lambertian<double>;
  using Metal = Metal<double>;
  using Color3 = Color3<double>;
  using Point3 = Point3<double>;

  Metal leftMat = Metal(Color3(0.6, 0.7, 0.6));
  Metal rightMat = Metal(Color3(0.4, 0.5, 0.7));
  Lambertian middleMat = Lambertian(Color3(0.6, 0.7, 0.6));
  Lambertian groundMat = Lambertian(Color3(0.8, 0.3, 0.6));

  Sphere<double> left(Point3(-1.1, 0, -1), 0.5, &leftMat);
  Sphere<double> middle(Point3(0, 0, -1), 0.5, &middleMat);
  Sphere<double> right(Point3(1.1, 0, -1), 0.5, &rightMat);
  Sphere<double> ground(Point3(0, -100.5, -1), 100, &groundMat);

  std::vector<Hittable<double> *> hittables = {&left, &middle, &right, &ground};
  Scene<double> scene(hittables);

  Image image("sphere.png", 600, 300, 3);

  const double viewportHeight = 2;
  Camera<double> camera(viewportHeight * image.aspectRatio, viewportHeight, 1.0,
                        Point3(0, 0, 0), 100, 1000);

  scene.render(camera, image);
}
