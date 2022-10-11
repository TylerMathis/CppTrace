#ifndef COMMON_H
#define COMMON_H

#include <random>

namespace common {

inline double randomDouble() { return rand() / (RAND_MAX + 1.0); }

inline double randomDouble(const double min, const double max) {
  return min + (max - min) * randomDouble();
}

inline int randomInt(const int min, const int max) {
  return randomDouble(min, max + 1);
}

inline double clamp(const double val, const double min, const double max) {
  if (val < min)
    return min;
  if (val > max)
    return max;
  return val;
}

} // namespace common

#endif