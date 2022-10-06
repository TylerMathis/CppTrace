#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <stdexcept>

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

  Vec3 unit() const { return *this / mag(); }
};

typedef Vec3 Point3;
typedef Vec3 Color3;

#endif