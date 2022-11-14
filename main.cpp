#include "./lib/scene/camera.hpp"
#include "./lib/scene/image.hpp"
#include "./lib/scene/scene.hpp"
#include "./lib/hittable/objects/stl.hpp"
#include "./lib/hittable/textures/solid_color.hpp"
#include "./lib/hittable/materials/lambertian.hpp"
#include "./lib/accelerators/accelerators.hpp"

#include <memory>

int main() {
  const double aspectRatio = 1;
  const int width = 250;
  const int height = (int) (width / aspectRatio);
  const int samples = 1;
  const int bounces = 5;
  Image image(R"(D:\code\proj\CppTrace\images\booh.png)", width, height, samples, bounces);

  Point3 origin(0, 50, 100);
  Point3 lookAt(0, 50, 0);
  Point3 up(0, 1, 0);
  const double aperture = 0;
  const double focusDist = (origin - lookAt).mag();
  const double fov = 60;
  Camera camera(origin, lookAt, up, fov, image.aspectRatio, aperture,
                focusDist);

  Scene scene(std::make_shared<Camera>(camera),
              std::make_shared<Image>(image), BVH_ACCEL);

  auto texture = std::make_shared<SolidColorTexture>(0.5, 0.5, 0.5);
  auto material = std::make_shared<Lambertian>(texture);
  auto stl = std::make_shared<STL>(R"(D:\code\proj\CppTrace\objects\Ghost_Booh_N1.stl)", material);

  scene.pushHittables(stl->hittableList.hittables);
  scene.setAmbient(Color3(0.5, 0.5, 0.5));

  scene.render(12);
}