#include "./lib/scene/camera.hpp"
#include "./lib/scene/image.hpp"
#include "./lib/scene/scene.hpp"
#include "./lib/scene/cornell_box.hpp"

#include <memory>

int main() {
  const double aspectRatio = 1;
  const int width = 500;
  const int height = (int) (width / aspectRatio);
  const int samples = 100;
  const int bounces = 50;
  Image image(R"(D:\code\proj\CppTrace\images\cornell.png)", width, height, samples, bounces);

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

  auto box = cornell_scene::cornellBox();
  auto prism = cornell_scene::prism();

  scene.pushHittables(box);
  scene.pushHittables(prism);

  scene.render();
}