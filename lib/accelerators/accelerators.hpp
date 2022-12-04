//
// Created by Tyler Hostler-Mathis on 11/11/22.
//

#ifndef CPPTRACE_LIB_ACCELERATORS_ACCELERATORS_HPP_
#define CPPTRACE_LIB_ACCELERATORS_ACCELERATORS_HPP_

#include <string>
#include <unordered_map>

enum ACCELERATOR {
  SIMPLE_LIST,
  KD_TREE_ACCEL,
  BVH_ACCEL,
  MADMAN_BVH,
  FAST_BVH,
};

ACCELERATOR accelFromString(std::string accelerator) {
  for (char &c : accelerator)
    c = (char)std::tolower(c);

  if (accelerator == "simple_list")
    return SIMPLE_LIST;
  if (accelerator == "kd_tree")
    return KD_TREE_ACCEL;
  else if (accelerator == "bvh")
    return BVH_ACCEL;
  else if (accelerator == "madman_bvh")
    return MADMAN_BVH;
  else if (accelerator == "fast_bvh")
    return FAST_BVH;

  throw std::invalid_argument("Invalid accelerator name in conversion: " +
                              accelerator);
}

std::string stringFromAccel(ACCELERATOR accelerator) {
  if (accelerator == SIMPLE_LIST)
    return "simple_list";
  else if (accelerator == KD_TREE_ACCEL)
    return "kd_tree";
  else if (accelerator == BVH_ACCEL)
    return "bvh";
  else if (accelerator == MADMAN_BVH)
    return "madman_bvh";
  else if (accelerator == FAST_BVH)
    return "fast_bvh";
  return "";
}

#endif // CPPTRACE_LIB_ACCELERATORS_ACCELERATORS_HPP_
