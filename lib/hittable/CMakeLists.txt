include(lib/hittable/bounding/CMakeLists.txt)
include(lib/hittable/materials/CMakeLists.txt)
include(lib/hittable/objects/CMakeLists.txt)
include(lib/hittable/textures/CMakeLists.txt)

list(APPEND CppTrace_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/hit.hpp
    ${CMAKE_CURRENT_LIST_DIR}/hittable.hpp
    ${CMAKE_CURRENT_LIST_DIR}/hittable_list.hpp
    )
