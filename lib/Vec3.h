#ifndef VEC3_H
#define VEC3_H

#include "Common.h"

#include <cmath>
#include <stdexcept>

template <typename T> struct _Vec3 {
  T x, y, z;

  _Vec3() : x(0), y(0), z(0) {}
  _Vec3(const T e0, const T e1, const T e2) : x(e0), y(e1), z(e2) {}

  template <typename T2> _Vec3(const _Vec3<T2> &o) {
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
      throw std::invalid_argument("_Vec3 index oob");
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
      throw std::invalid_argument("_Vec3 index oob");
    }
  }

  _Vec3 operator-() const { return _Vec3(-x, -y, -z); }

  _Vec3 operator+(const _Vec3 &o) const {
    return _Vec3(x + o.x, y + o.y, z + o.z);
  }
  _Vec3 operator-(const _Vec3 &o) const {
    return _Vec3(x - o.x, y - o.y, z - o.z);
  }
  _Vec3 operator*(const _Vec3 &o) const {
    return _Vec3(x * o.x, y * o.y, z * o.z);
  }
  _Vec3 operator+(const double s) const { return _Vec3(x + s, y + s, z + s); }
  _Vec3 operator-(const double s) const { return _Vec3(x - s, y - s, z - s); }
  _Vec3 operator*(const double s) const { return _Vec3(x * s, y * s, z * s); }
  _Vec3 operator/(const double s) const { return _Vec3(x / s, y / s, z / s); }

  _Vec3 &operator+=(const _Vec3 &o) {
    x += o.x, y += o.y, z += o.z;
    return *this;
  }
  _Vec3 &operator-=(const _Vec3 &o) {
    x -= o.x, y -= o.y, z -= o.z;
    return *this;
  }
  _Vec3 &operator*=(const _Vec3 &o) {
    x *= o.x, y *= o.y, z *= o.z;
    return *this;
  }
  _Vec3 &operator*=(const double s) {
    x *= s, y *= s, z *= s;
    return *this;
  }
  _Vec3 &operator/=(const double s) {
    x /= s, y /= s, z /= s;
    return *this;
  }

  _Vec3 cross(const _Vec3 &o) const {
    return _Vec3(y * o.z - z * o.y, z * o.x - x * o.z, x * o.y - y * o.x);
  }

  double dot(const _Vec3 &o) const { return x * o.x + y * o.y + z * o.z; }
  double mag2() const { return x * x + y * y + z * z; }
  double mag() const { return std::sqrt(mag2()); }
  double dist(const _Vec3 &o) const { return (*this - o).mag(); }

  _Vec3 unit() const { return *this / mag(); }
  _Vec3 sqrt() const { return _Vec3(std::sqrt(x), std::sqrt(y), std::sqrt(z)); }
  _Vec3 abs() const { return _Vec3(std::abs(x), std::abs(y), std::abs(z)); }

  _Vec3 reflect(const _Vec3 &axis) const {
    _Vec3 vec = *this;
    return vec - axis * 2 * vec.dot(axis);
  }

  _Vec3 refract(const _Vec3 &normal, const double refractionRatio) const {
    _Vec3 vec = *this;

    double cosTheta = std::min(-vec.dot(normal), 1.0);
    _Vec3 outPerp = (vec + normal * cosTheta) * refractionRatio;
    _Vec3 outPara = normal * -std::sqrt(std::abs(1.0 - outPerp.mag2()));

    return outPerp + outPara;
  }

  inline static _Vec3 random() {
    return _Vec3(common::randomDouble(), common::randomDouble(),
                 common::randomDouble());
  }

  inline static _Vec3 random(const T min, const T max) {
    return _Vec3(common::randomDouble(min, max), common::randomDouble(min, max),
                 common::randomDouble(min, max));
  }

  static _Vec3 randomInUnitSphere() {
    while (true) {
      _Vec3 p = random(-1, 1);
      if (p.mag2() >= 1)
        continue;
      return p;
    }
  }

  static _Vec3 randomInHemisphere(const _Vec3 &normal) {
    _Vec3 inUnitSphere = randomInUnitSphere();
    if (inUnitSphere.dot(normal) > 0.0)
      return inUnitSphere;
    return -inUnitSphere;
  }

  static _Vec3 randomInUnitDisk() {
    while (true) {
      _Vec3 p(common::randomDouble(-1, 1), common::randomDouble(-1, 1), 0);
      if (p.mag2() >= 1)
        continue;
      return p;
    }
  }
};

template <typename T = double> using _Point3 = _Vec3<T>;
template <typename T = double> using _Color3 = _Vec3<T>;
template <typename T = double> using _Normal = _Vec3<T>;

using Vec3 = _Vec3<double>;
using Point3 = _Point3<double>;
using Color3 = _Color3<double>;
using Normal = _Normal<double>;

#endif
