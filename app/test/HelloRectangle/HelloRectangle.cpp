// HelloRectangle.cpp
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw3native.h>

#include <cstdint>

#include <iostream>
#include <vector>
#include <string>
#include <array>

#include <vkw.hpp>

#include "Presentation.hpp"

/**
 * A rectangle app
*/
class HelloRectangle
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

  const int MAX_FRAMES_IN_FLIGHT = 2;

  const std::vector<vkw::Vertex> vertices =
  {
    {{0.0f, -0.5f}, {1.0f, 1.0f, 1.0f}},
    {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    //{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    //{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    //{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    //{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
  };

  const std::vector<uint16_t> indices =
  {0, 1, 2, 2, 3, 0};

  static void framebufferResizeCallback(GLFWwindow *window, int width, int height)
  {
    auto app = reinterpret_cast<HelloRectangle*>(glfwGetWindowUserPointer(window));
    app->m_context.framebuffer_resized = true;
  }

  void initWindow()
  {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    m_window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);
  }

  std::vector<const char*> getRequiredExtensions()
  {
    uint32_t glfw_extension_count = 0;
    const char **glfw_extensions =
      glfwGetRequiredInstanceExtensions(&glfw_extension_count);
    
    std::vector<const char*> extensions(glfw_extensions, glfw_extensions + 
      glfw_extension_count);
    
    if (vkw::Validation::enable_validation_layers)
    {
      extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
  }

  void createSurface()
  {
    if (glfwCreateWindowSurface
    (
      m_context.instance, 
      m_window, 
      nullptr, 
      &m_context.surface
    ) != VK_SUCCESS)
    {
      throw std::runtime_error("failed to create window surface!");
    }
  }

  void createSwapChain()
  {
    std::array<int, 2> buffer_size
      = Presentation::updateFramebufferSize(m_window);

    vkw::Swapchain::createSwapchain
    (
      buffer_size[0],
      buffer_size[1],
      &m_context
    );
  }

  void createGraphicsPipeline()
  {
    std::vector<std::string> vert_shader_files = 
      {"vert.spv"};
    std::vector<std::string> frag_shader_files =
      {"frag.spv"};
    
    vkw::GraphicsPipeline::createGraphicsPipeline
    (
      m_context,
      vert_shader_files,
      frag_shader_files,
      vkw::Vertex::getBindingDescription(),
      vkw::Vertex::getAttributeDescriptions()
    );
  }

  void initVulkan()
  {
    vkw::Instance::createInstance
    (
      &m_context.instance,
      "Hello Rectangle",
      (int[]){1,0,0},
      getRequiredExtensions()
    );
    vkw::Validation::setupDebugMessenger(m_context);
    createSurface();
    vkw::PhysicalDevice::pickPhysicalDevice(m_context);
    vkw::LogicalDevice::createLogicalDevice(m_context);
    createSwapChain();
    vkw::Swapchain::createImageViews();
    vkw::RenderPass::createRenderPass(m_context);
    createGraphicsPipeline();
    vkw::Framebuffer::createFramebuffers(m_context);
    vkw::Command::createCommandPool(m_context);
    vkw::VertexBuffer::createVertexBuffer
    (
      m_context,
      vertices
    );
    vkw::Command::createCommandBuffers
    (
      m_context.command_buffers,
      MAX_FRAMES_IN_FLIGHT
    );
    vkw::Synchronization::createSyncObjects
    (
      m_context,
      &MAX_FRAMES_IN_FLIGHT
    );
  }

  void mainLoop()
  {
    while (!glfwWindowShouldClose(m_window))
    {
      glfwPollEvents();
      Presentation::drawFrame
      (
        m_context,
        MAX_FRAMES_IN_FLIGHT,
        m_window,
        vertices
      );
    }

    vkDeviceWaitIdle(m_context.logical_device);
  }

  void cleanup()
  {
    vkw::Swapchain::cleanupSwapchain();

    vkw::VertexBuffer::destroyVertexBuffer();
    vkw::GraphicsPipeline::destroyGraphicsPipeline();
    vkw::RenderPass::destroyRenderPass();
    vkw::Synchronization::destroySyncObjects();
    vkw::Command::destroyCommandPool();
    vkw::LogicalDevice::destroyLogicalDevice();

    vkw::Validation::destroyDebugUtilsMessengerEXT();

    vkDestroySurfaceKHR(m_context.instance, m_context.surface, nullptr);
    vkw::Instance::destroyInstance();

    glfwDestroyWindow(m_window);

    glfwTerminate();
  }
};

int main(int argc, char *argv[])
{
  HelloRectangle hr;

  try 
  {
    hr.run();
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}