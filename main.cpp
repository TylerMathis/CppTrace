#include "./lib/Camera.h"
#include "./lib/Image.h"
#include "./lib/Scene/RandomGen.h"
#include "./lib/Scene/Scene.h"
#include "./lib/Vec3.h"

int main() {
  const double aspectRatio = 16.0 / 9.0;
  const int width = 500;
  const int height = width / aspectRatio;
  const int samples = 200;
  const int bounces = 50;
  Image image("manyObjects", width, height, samples, bounces);

  Point3 origin(0, 8, 20);
  Point3 lookAt(0, 0, 0);
  Point3 up(0, 1, 0);
  const double aperture = 0.2;
  const double focusDist = (origin - lookAt).mag();
  const double fov = 60;
  Camera camera(origin, lookAt, up, fov, image.aspectRatio, aperture,
                focusDist);

  auto world = randomScene(10000);
  Scene scene(camera, image, world);
  scene.render();
}
