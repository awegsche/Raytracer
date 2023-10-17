# CMakeList.txt : CMake project for Raytracer, include source and define
# project specific logic here.
#
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
include_directories("${OptiX_INCLUDE}")

# Add source to this project's executable.
add_executable (Raytracer
	"Raytracer.cpp"
    "utils/optix_helpers.cpp"
    "ui/application.cpp"
    "ui/cu_application.cpp"
	"utils/cuda_helpers.cpp")

target_link_libraries(Raytracer
	GLEW::GLEW
	glfw
	imgui::imgui
	dear_spdlogger
	spdlog::spdlog
	CUDA::cuda_driver
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

set(MODULE_TARGET_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$(ConfigurationName)")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")

# TODO: Add tests and install targets if needed.
