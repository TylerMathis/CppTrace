//
// Created by Tyler Hostler-Mathis on 10/18/22.
//

#ifndef RAYTRACER_LIB_COMMON_VEC3_HPP_
#define RAYTRACER_LIB_COMMON_VEC3_HPP_

#include "common.hpp"

#include <cmath>
#include <stdexcept>

struct Vec3 {
  double x, y, z;

  Vec3() : x(0), y(0), z(0) {}
  Vec3(const double e0, const double e1, const double e2)
      : x(e0), y(e1), z(e2) {}

  double operator[](const int i) const {
    switch (i) {
      case 0:return x;
      case 1:return y;
      case 2:return z;
      default:throw std::invalid_argument("Vec3 index oob");
    }
  }

  double &operator[](const int i) {
    switch (i) {
      case 0:return x;
      case 1:return y;
      case 2:return z;
      default:throw std::invalid_argument("Vec3 index oob");
    }
  }

  Vec3 operator-() const { return {-x, -y, -z}; }

  Vec3 operator+(const Vec3 &o) const {
    return {x + o.x, y + o.y, z + o.z};
  }
  Vec3 operator-(const Vec3 &o) const {
    return {x - o.x, y - o.y, z - o.z};
  }
  Vec3 operator*(const Vec3 &o) const {
    return {x * o.x, y * o.y, z * o.z};
  }
  Vec3 operator+(const double s) const { return {x + s, y + s, z + s}; }
  Vec3 operator-(const double s) const { return {x - s, y - s, z - s}; }
  Vec3 operator*(const double s) const { return {x * s, y * s, z * s}; }
  Vec3 operator/(const double s) const { return {x / s, y / s, z / s}; }

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

  [[nodiscard]] Vec3 cross(const Vec3 &o) const {
    return {y * o.z - z * o.y, z * o.x - x * o.z, x * o.y - y * o.x};
  }

  [[nodiscard]] double dot(const Vec3 &o) const {
    return x * o.x + y * o.y + z * o.z;
  }
  [[nodiscard]] double mag2() const { return x * x + y * y + z * z; }
  [[nodiscard]] double mag() const { return std::sqrt(mag2()); }
  [[nodiscard]] double dist(const Vec3 &o) const { return (*this - o).mag(); }

  [[nodiscard]] Vec3 unit() const { return *this / mag(); }
  [[nodiscard]] Vec3 sqrt() const {
    return {std::sqrt(x), std::sqrt(y), std::sqrt(z)};
  }
  [[nodiscard]] Vec3 abs() const {
    return {std::abs(x), std::abs(y), std::abs(z)};
  }

  [[nodiscard]] Vec3 reflect(const Vec3 &axis) const {
    Vec3 vec = *this;
    return vec - axis * 2 * vec.dot(axis);
  }

  [[nodiscard]] Vec3 refract(const Vec3 &normal,
                             const double refractionRatio) const {
    Vec3 vec = *this;

    double cosTheta = std::min(-vec.dot(normal), 1.0);
    Vec3 outPerp = (vec + normal * cosTheta) * refractionRatio;
    Vec3 outPara = normal * -std::sqrt(std::abs(1.0 - outPerp.mag2()));

    return outPerp + outPara;
  }

  inline static Vec3 random() {
    return {common::randomDouble(), common::randomDouble(),
            common::randomDouble()};
  }

  inline static Vec3 random(const double min, const double max) {
    return {common::randomDouble(min, max), common::randomDouble(min, max),
            common::randomDouble(min, max)};
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

  static Vec3 randomInUnitDisk() {
    while (true) {
      Vec3 p(common::randomDouble(-1, 1), common::randomDouble(-1, 1), 0);
      if (p.mag2() >= 1)
        continue;
      return p;
    }
  }
};

using Point3 = Vec3;
using Color3 = Vec3;
using Normal = Vec3;

#endif //RAYTRACER_LIB_COMMON_VEC3_HPP_
