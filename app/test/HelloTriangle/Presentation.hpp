// Presentation.hpp
#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw3native.h>

#include <vector>
#include <array>

#include <vkw.hpp>

class Presentation
{
public:
  static void drawFrame
  (
    vkw::Context &context,
    const int &flight_frame_count,
    GLFWwindow *window,
    std::vector<vkw::Vertex> &vertices
  );
  static std::array<int, 2> updateFramebufferSize
  (
    GLFWwindow *window
  );
};