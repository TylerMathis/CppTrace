cmake_minimum_required(VERSION 3.23)
project(raytracer)

set(CMAKE_CXX_STANDARD 17)

add_executable(raytracer main.cpp lib/extern/obj_loader.hpp lib/extern/stb_image_write.hpp lib/common/vec3.hpp lib/common/common.hpp lib/cli/progress_indicator.hpp lib/common/ray.hpp lib/scene/camera.hpp lib/hittable/hittable.hpp lib/hittable/hit.hpp lib/hittable/material/material.hpp lib/hittable/material/dielectric.hpp lib/hittable/material/lambertian.hpp lib/hittable/material/metal.hpp lib/hittable/bounding/aabb.hpp lib/hittable/bounding/bvh.hpp lib/hittable/hittable_list.hpp lib/hittable/obj.hpp lib/hittable/triangle.hpp lib/hittable/sphere.hpp lib/scene/image.hpp lib/scene/random_gen.hpp lib/scene/scene.hpp lib/scene/scene_tools.hpp)
