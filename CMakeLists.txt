cmake_minimum_required(VERSION 3.22)
project(raytracer)

set(CMAKE_CXX_STANDARD 17)

add_executable(raytracer main.cpp lib/extern/obj_loader.hpp lib/extern/stb_image_write.hpp lib/common/vec3.hpp lib/common/common.hpp lib/cli/progress_indicator.hpp lib/common/ray.hpp lib/scene/camera.hpp lib/hittable/hittable.hpp lib/hittable/hit.hpp lib/hittable/materials/material.hpp lib/hittable/materials/dielectric.hpp lib/hittable/materials/lambertian.hpp lib/hittable/materials/metal.hpp lib/hittable/bounding/aabb.hpp lib/hittable/bounding/bvh.hpp lib/hittable/hittable_list.hpp lib/hittable/objects/obj.hpp lib/hittable/objects/triangle.hpp lib/hittable/objects/sphere.hpp lib/scene/image.hpp lib/scene/random_gen.hpp lib/scene/scene.hpp lib/scene/scene_tools.hpp lib/hittable/materials/emissive.hpp lib/hittable/objects/rectangle.hpp lib/scene/cornell_box.hpp lib/hittable/textures/texture.hpp lib/hittable/textures/solid_color.hpp lib/hittable/textures/image.hpp lib/extern/stb_image_read.hpp lib/hittable/objects/rectangular_prism.hpp lib/hittable/materials/isotropic.hpp lib/hittable/objects/constant_medium.hpp)
