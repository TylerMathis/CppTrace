#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "stdint.h"
#include "stdlib.h"

#include <cmath>
#include <stdexcept>
#include <string>

struct Vec3 {
  double x, y, z;
  Vec3() : x(0), y(0), z(0) {}
  Vec3(const double e0, const double e1, const double e2)
      : x(e0), y(e1), z(e2) {}

  double operator[](const int i) const {
    switch (i) {
    case 0:
      return x;
    case 1:
      return y;
    case 2:
      return z;
    default:
      throw std::invalid_argument("Vec3 index oob");
    }
  }

  double &operator[](const int i) {
    switch (i) {
    case 0:
      return x;
    case 1:
      return y;
    case 2:
      return z;
    default:
      throw std::invalid_argument("Vec3 index oob");
    }
  }

  Vec3 operator-() const { return Vec3(-x, -y, -z); }

  Vec3 operator+(const Vec3 &o) const {
    return Vec3(x + o.x, y + o.y, z + o.z);
  }
  Vec3 operator-(const Vec3 &o) const {
    return Vec3(x - o.x, y - o.y, z - o.z);
  }
  Vec3 operator*(const double s) const { return Vec3(x * s, y * s, z * s); }
  Vec3 operator/(const double s) const { return Vec3(x / s, y / s, z / s); }

  Vec3 &operator+=(const Vec3 &o) {
    x += o.x, y += o.y, z += o.z;
    return *this;
  }
  Vec3 &operator-=(const Vec3 &o) {
    x -= o.x, y -= o.y, z -= o.z;
    return *this;
  }
  Vec3 &operator*=(const double s) {
    x *= s, y *= s, z *= s;
    return *this;
  }
  Vec3 &operator/=(const double s) {
    x /= s, y /= s, z /= s;
    return *this;
  }

  Vec3 cross(const Vec3 &o) const {
    return Vec3(y * o.z - z * o.y, x * o.z - z * o.x, x * o.y - y * o.x);
  }
  double dot(const Vec3 &o) const { return x * o.x + y * o.y + z * o.z; }

  double mag2() const { return x * x + y * y + z * z; }
  double mag() const { return std::sqrt(mag2()); }
  double dist(const Vec3 &o) const { return (*this - o).mag(); }
};

typedef Vec3 Point3;
typedef Vec3 Color3;

struct Image {
  const char *name;
  int width, height, comps, byte = 0;
  uint8_t *data;
  Image(const std::string _name, const int _width, const int _height,
        const int _comps) {
    name = _name.c_str();
    width = _width, height = _height, comps = _comps;
    data = (uint8_t *)calloc(comps * width * height, sizeof(uint8_t));
  }
  void pushPixel(Color3 c) {
    for (int i = 0; i < 3; i++)
      data[byte + i] = c[i];
    byte += 3;
  }
  void write() {
    stbi_write_png(name, width, height, comps, data, width * comps);
  }
};

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
