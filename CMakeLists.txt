# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("Raytracer")

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/OptiX_SDK/SDK/CMake")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(CUDAToolkit 10.0 REQUIRED)

set(OptiX_INSTALL_DIR "${CMAKE_SOURCE_DIR}/OptiX_SDK" CACHE PATH "Path to OptiX installed location.")
message("optix install dir: ${OptiX_INSTALL_DIR}")
find_package(OptiX REQUIRED)

find_package(imgui REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_subdirectory(ext/dear_spdlog)

message("optix include dir: ${OptiX_INCLUDE_DIR}")
message("optix include: ${OptiX_INCLUDE}")
message("glfw include: ${GLFW_INCLUDE_DIR}")
message("glfw include: ${GLFW_INCLUDE_DIRS}")
include_directories(
	"ext/dear_spdlog/src"
	${GLEW_INCLUDE_DIRS}
	${OPTIX_INCLUDE}
	${CUDAToolkit_INCLUDE_DIRS}
)

# OptiX setup
set(USE_OPTIX_IR TRUE)
set(OPTIX_MODULE_EXTENSION ".optixir")
set(OPTIX_PROGRAM_TARGET "--optix-ir")

#sglet(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /doc")

# Include sub-projects.
add_subdirectory ("Raytracer")
