#include "./lib/scene/camera.hpp"
#include "./lib/hittable/material/dielectric.hpp"
#include "./lib/hittable/material/lambertian.hpp"
#include "./lib/hittable/material/emissive.hpp"
#include "./lib/hittable/material/metal.hpp"
#include "./lib/hittable/obj.hpp"
#include "./lib/hittable/sphere.hpp"
#include "./lib/scene/image.hpp"
#include "./lib/scene/scene.hpp"
#include "./lib/scene/random_gen.hpp"

#include <memory>

int main() {
  const double aspectRatio = 16.0 / 9.0;
  const int width = 800;
  const int height = (int) (width / aspectRatio);
  const int samples = 200;
  const int bounces = 10;
  Image image("emissive", width, height, samples, bounces, 0.001, 100);

  Point3 origin(20, 20, -20);
  Point3 lookAt(0, 0, 0);
  Point3 up(0, 1, 0);
  const double aperture = 0;
  const double focusDist = (origin - lookAt).mag();
  const double fov = 60;
  Camera camera(origin, lookAt, up, fov, image.aspectRatio, aperture,
                focusDist);

  Scene scene(std::make_shared<Camera>(camera),
              std::make_shared<Image>(image));
  auto world = randomScene(1000);
  scene.loadHittables(world);
  scene.setAmbient(Color3(0.4, 0.6, 0.8));
  scene.render();
}