// HelloTriangle.cpp
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw3native.h>

#include <iostream>

#include "Context.hpp"

/**
 * A triangle app
*/
class HelloTriangle
{
public:
  void run()
  {
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
  }
  
private:
  GLFWwindow *m_window;
  vkw::Context m_context;

  const uint32_t WIDTH = 800;
  const uint32_t HEIGHT = 600;

  void initWindow()
  {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
  }

  void initVulkan()
  {

  }

  void mainLoop()
  {
    while (!glfwWindowShouldClose(m_window))
    {
      glfwPollEvents();
    }
  }

  void cleanup()
  {
    glfwDestroyWindow(m_window);

    glfwTerminate();
  }
};

int main(int argc, char *argv[])
{
  HelloTriangle ht;

  try
  {
    ht.run();
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}