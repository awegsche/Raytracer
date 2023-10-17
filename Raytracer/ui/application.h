#pragma once

#ifdef _WIN32
#include <Windows.h>
#endif

#include <chrono>
#include <cuda.h>
#include <optix.h>
#include <iostream>
#include <memory>

#include <dear_sink.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <spdlog/spdlog.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "utils/timer.h"
#include "ui/cu_application.h"

using std::cout, std::cerr, std::endl;
using std::chrono::steady_clock;

class Application {
    // window
    GLFWwindow *m_window = nullptr;
    GLsizei m_window_width;
    GLsizei m_window_height;
    // "docking"
    float left_margin   = 256.0f;
    float right_margin  = 0.0f;
    float bottom_margin = 512.0f;

    bool m_glfw_valid = false;
    bool m_is_valid   = false;
    bool render_gpu   = true;

    // debug
    dear_sink_mt_t m_sink;
    FpsCounter m_fps_counter;
    float m_fps;

    std::unique_ptr<CUApplication> m_cu_application;

    // debugging, this should be moved into subclasses
    float m_cam_fov = 90.0f;

  public:
    // ---- Init and Destruction -------------------------------------------------------------------

    /// @brief
    /// The **main** <b>application</b>, _hosts_ *the* GLFW window, OpenGL and CUDA contexts and the renderer
    /// @param width the width
    /// @param height the window height
    Application(int width, int height);
    ~Application();

    // ---- Frames ---------------------------------------------------------------------------------

    /// <summary>
    /// Starts frame drawing. Sets up everything for imgui and draws common ui elements
    ///
    /// <em>Note</em>: this does not render the imgui buffer to the OpenGL window.
    /// This is done in <c>end_frame()</c>.
    /// </summary>
    /// <returns></returns>
    void begin_frame();

    /// <summary>
    /// Ends frame drawing.
    /// </summary>
    /// <returns></returns>
    void end_frame() const;

    void get_system_information();

    auto should_close() const -> bool { return glfwWindowShouldClose(m_window); }

    auto is_valid() const -> bool { return m_is_valid; }
};
