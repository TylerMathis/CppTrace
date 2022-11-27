//
// Created by Tyler Hostler-Mathis on 11/11/22.
//

#ifndef CPPTRACE_LIB_ACCELERATORS_ACCELERATORS_HPP_
#define CPPTRACE_LIB_ACCELERATORS_ACCELERATORS_HPP_

#include <string>
#include <unordered_map>

enum ACCELERATOR {
  BVH_ACCEL,
  MADMAN_BVH,
};

ACCELERATOR accelFromString(std::string accelerator) {
  for (char &c : accelerator)
    c = (char) std::tolower(c);

  if (accelerator == "bvh")
    return BVH_ACCEL;
  else if (accelerator == "madman_bvh")
    return MADMAN_BVH;

  throw std::invalid_argument("Invalid accelerator name in conversion: " + accelerator);
}

std::string stringFromAccel(ACCELERATOR accelerator) {
  if (accelerator == BVH_ACCEL)
    return "bvh";
  else if (accelerator == MADMAN_BVH)
    return "madman_bvh";
}

#endif //CPPTRACE_LIB_ACCELERATORS_ACCELERATORS_HPP_
