#include "./lib/scene/camera.hpp"
#include "./lib/scene/image.hpp"
#include "./lib/scene/scene.hpp"
#include "./lib/scene/cornell_box.hpp"
#include "./lib/hittable/objects/sphere.hpp"
#include "./lib/hittable/textures/image.hpp"

#include <memory>

int main() {
  const double aspectRatio = 1;
  const int width = 400;
  const int height = (int) (width / aspectRatio);
  const int samples = 100;
  const int bounces = 20;
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

  // Load the skeleton
  auto box = cornellBox();
  scene.loadHittables(box);

  auto earthTexture = std::make_shared<ImageTexture>("textures/earthmap.jpeg");
  auto earthMat = std::make_shared<Lambertian>(earthTexture);
  auto earth = std::make_shared<Sphere>(Point3(0, 1, 0), 0.5, earthMat);
  scene.pushHittable(earth);

  scene.setAmbient(Color3(0.5, 0.5, 0.5));
  scene.render();
}