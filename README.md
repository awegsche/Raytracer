# OptiX Raytracer

A Raytracer written in C++ and CUDA using NVidia's OptiX raytracing pipeline.

## Setup

### Prerequisites

**Note:** This is only tested on Windows (Win10 and 11), couldn't try it yet on any other platform.

#### **Main Prerequisites**

- Any version of Visual Studio starting from 2019.
_Note: Also CLion worked fine, I didn't get to the point of debugging the CUDA code yet, though_
- [CUDA Toolkit](https://developer.nvidia.com/cuda-toolkit), version 10+
- [OptiX SDK](https://developer.nvidia.com/rtx/ray-tracing/optix) version 7.6+,
    extract it into this source dir and rename it to `OptiX_SDK`

#### **External Packages**

- GLEW and GLFW for the OpenGL window handling
- Dear Imgui for GUI
- spdlog for logging

### Instructions

With the [main prerequisites](#main-prerequisites) installed, we need to follow the following steps:

For this, environment variables need to be present:
- `CUDA_PATH` pointing to the install dir of the CUDA toolkit

#### Then, try get external packages.
There are two ways of doing that:

1. C++ package manager (conan, vcpkg, etc)
2. Git submodules

I never really figured out how any of these work in a more complex project in
a sufficiently general manner.

For this project I went with vcpkg for the more 'standard' packages and an
empirical mix of vcpkg and git submodules for the rest.
Here it is:

- If not already present, install vcpkg following their installation intructions (basically, clone the repo, run bootstrap).
Optionally, add the directory to `PATH`.
- Install GLEW and GLFW:
```
vcpkg install GLEW:x64-windows
vcpkg install glfw3:x64-windows
```
- Install spdlog:
```
vcpkg install spdlog:x64-windows
```
- Install imgui:
```
vcpkg install imgui:x64-windows
```
_Note_: possibly you need to specify the backends:
```
vcpkg install imgui[core,opengl3-binding,glfw-binding]:x64-windows
```
_2nd Note:_ optionally you can decide to install imgui as git submodule and checkout the `docking` branch.

- Then checkout my dear spdlog repo
```
cd ext
git submodule add git@github.com:awegsche/dear_spdlog.git
```

