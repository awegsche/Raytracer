# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("Raytracer")

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/ext/CMake")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(CUDAToolkit 10.0 REQUIRED)
find_package(OptiX76 REQUIRED)
find_package(imgui REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_subdirectory(ext/dear_spdlog)

message("optix include: ${OPTIX_INCLUDE_DIR}")
include_directories(
	"ext/dear_spdlog/src"
	${GLEW_INCLUDE_DIRS}
	${GLFW_INCLUDE_DIR}
	${OPTIX_INCLUDE_DIR}
	${CUDAToolkit_INCLUDE_DIRS}
)

# OptiX setup
set(USE_OPTIX_IR TRUE)
set(OPTIX_MODULE_EXTENSION ".optixir")
set(OPTIX_PROGRAM_TARGET "--optix-ir")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /doc")

# Include sub-projects.
add_subdirectory ("Raytracer")
