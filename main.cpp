#include "./lib/Camera.h"
#include "./lib/Image.h"

int main() {
  Image image("test.png", 1000, 500, 3);

  const double viewportHeight = 2.0;
  Camera<> camera(viewportHeight * image.aspectRatio, viewportHeight, 1.0,
                  Point3<>(0, 0, 0));

  Circle<> c(Point3<>(0, 0, 1), 0.5);
  camera.pushPrimitive(c);

  for (int j = 0; j < image.height; j++) {
    for (int i = 0; i < image.width; i++) {
      double x = double(i) / (image.width - 1);
      double y = double(j) / (image.height - 1);
      auto color = camera.getColor(x, y);
      image.pushPixel(color * 255);
    }
  }

  image.write();
}
