#include "./lib/Image.h"
#include "./lib/Vec3.h"

int main() {
  Image image("test.png", 256, 256, 3);

  for (int j = 0; j < image.height; j++) {
    for (int i = 0; i < image.width; i++) {
      Color3 color(double(i) / (image.width - 1),
                   double(j) / (image.height - 1), 0.25);

      image.pushPixel(color * 255);
    }
  }

  image.write();
}
