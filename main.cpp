#include "./lib/scene/camera.hpp"
#include "./lib/scene/image.hpp"
#include "./lib/scene/scene.hpp"
#include "./lib/scene/cornell_box.hpp"

#include <memory>

int main() {
  const double aspectRatio = 1;
  const int width = 400;
  const int height = (int) (width / aspectRatio);
  const int samples = 20;
  const int bounces = 10;
  Image image("cornell", width, height, samples, bounces, 0.001, 100);

  Point3 origin(0, 1, 2.7);
  Point3 lookAt(0, 1, 0);
  Point3 up(0, 1, 0);
  const double aperture = 0;
  const double focusDist = (origin - lookAt).mag();
  const double fov = 60;
  Camera camera(origin, lookAt, up, fov, image.aspectRatio, aperture,
                focusDist);

  Scene scene(std::make_shared<Camera>(camera),
              std::make_shared<Image>(image));
  auto box = cornellBox();
  scene.loadHittables(box);
  scene.setAmbient(Color3(0.5, 0.5, 0.5));
  scene.render();
}