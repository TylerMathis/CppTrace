#include "./lib/Camera.h"
#include "./lib/Image.h"
#include "./lib/Primitive/Sphere.h"

int main() {
  Image image("spheres.png", 1000, 500, 3);

  const double viewportHeight = 2.0;
  Camera<double> camera(viewportHeight * image.aspectRatio, viewportHeight, 1.0,
                        Point3<double>(0, 0, 0));

  Sphere<double> c1(Point3<double>(0, 0, -1), 0.5);
  Sphere<double> c2(Point3<double>(0, 0.5, -1), 0.25);
  camera.pushPrimitive(&c1);
  camera.pushPrimitive(&c2);

  for (int j = 0; j < image.height; j++) {
    for (int i = 0; i < image.width; i++) {
      double x = double(i) / (image.width - 1);
      double y = double(j) / (image.height - 1);
      auto color = camera.getColor(x, y) * 255;
      Color3<uint8_t> integralColor = {static_cast<uint8_t>(color.x),
                                       static_cast<uint8_t>(color.y),
                                       static_cast<uint8_t>(color.z)};
      image.pushPixel(integralColor);
    }
  }

  image.write();
}
