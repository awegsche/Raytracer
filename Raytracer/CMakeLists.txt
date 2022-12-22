# CMakeList.txt : CMake project for Raytracer, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (Raytracer "Raytracer.cpp" "Raytracer.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Raytracer PROPERTY CXX_STANDARD 23)
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

set(MODULE_TARGET_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$(ConfigurationName)")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")

find_package(OpenGL REQUIRED)
find_package(GLFW REQUIRED)
find_package(GLEW REQUIRED)
find_package(CUDAToolkit 10.0 REQUIRED)
find_package(OptiX76)
# TODO: Add tests and install targets if needed.
