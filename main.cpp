#include "./lib/Camera.h"
#include "./lib/Hittable/Sphere.h"
#include "./lib/Image.h"
#include "./lib/Scene.h"

int main() {
  Scene<double> scene;
  Sphere<double> c1(Point3<double>(0, 0, -1), 0.5);
  Sphere<double> c2(Point3<double>(0, -100.5, -1), 100);
  scene.loadHittables({&c1, &c2});

  Image image("sphere.png", 500, 300, 3);

  const double viewportHeight = 2;
  Camera<double> camera(viewportHeight * image.aspectRatio, viewportHeight, 1.0,
                        Point3<double>(0, 0, 0), 25, 100);

  scene.render(camera, image);
}
