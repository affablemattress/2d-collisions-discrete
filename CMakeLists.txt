# CMakeList.txt : CMake project for 2d-collisions-discrete, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("2d-collisions-discrete")

# Add source to this project's executable.
add_executable (2d-collisions-discrete "main.cpp"  "LOG.h" "circle.cpp" "circle.h"   "V2.cpp" "V2.h")

target_include_directories(2d-collisions-discrete PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(2d-collisions-discrete "${PROJECT_SOURCE_DIR}/lib/raylib.lib" winmm.lib)
# TODO: Add tests and install targets if needed.
