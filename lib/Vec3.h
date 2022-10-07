#ifndef VEC3_H
#define VEC3_H

#include "Common.h"

#include <cmath>
#include <stdexcept>

template <typename T> struct Vec3 {
  T x, y, z;

  Vec3() : x(0), y(0), z(0) {}
  Vec3(const T e0, const T e1, const T e2) : x(e0), y(e1), z(e2) {}

  template <typename T2> Vec3(const Vec3<T2> &o) {
    x = static_cast<T>(o.x);
    y = static_cast<T>(o.y);
    z = static_cast<T>(o.z);
  }

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
  Vec3 operator*(const Vec3 &o) const {
    return Vec3(x * o.x, y * o.y, z * o.z);
  }
  Vec3 operator+(const double s) const { return Vec3(x + s, y + s, z + s); }
  Vec3 operator-(const double s) const { return Vec3(x - s, y - s, z - s); }
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
  Vec3 &operator*=(const Vec3 &o) {
    x *= o.x, y *= o.y, z *= o.z;
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
  Vec3 sqrt() const { return Vec3(std::sqrt(x), std::sqrt(y), std::sqrt(z)); }

  inline static Vec3 random() {
    return Vec3(common::randomDouble(), common::randomDouble(),
                common::randomDouble());
  }

  inline static Vec3 random(const T min, const T max) {
    return Vec3(common::randomDouble(min, max), common::randomDouble(min, max),
                common::randomDouble(min, max));
  }

  static Vec3 randomInUnitSphere() {
    while (true) {
      Vec3 p = random(-1, 1);
      if (p.mag2() >= 1)
        continue;
      return p;
    }
  }

  static Vec3 randomInHemisphere(const Vec3 &normal) {
    Vec3 inUnitSphere = randomInUnitSphere();
    if (inUnitSphere.dot(normal) > 0.0)
      return inUnitSphere;
    return -inUnitSphere;
  }
};

template <typename T = double> using Point3 = Vec3<T>;
template <typename T = double> using Color3 = Vec3<T>;
template <typename T = double> using Normal = Vec3<T>;

#endif