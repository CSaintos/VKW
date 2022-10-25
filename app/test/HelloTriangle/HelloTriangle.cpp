// HelloTriangle.cpp
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw3native.h>

#include <cstdint>

#include <iostream>
#include <vector>
#include <string>

#include <vkw.hpp>

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
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
  }

  std::vector<const char*> getRequiredExtensions()
  {
    uint32_t glfw_extension_count = 0;
    const char **glfw_extensions = 
      glfwGetRequiredInstanceExtensions(&glfw_extension_count);

    std::vector<const char*> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);

    if (vkw::Validation::enable_validation_layers)
    {
      extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
  }

  void createSurface()
  {
    if (glfwCreateWindowSurface(m_context.instance, m_window, nullptr, &m_context.surface) != VK_SUCCESS)
    {
      throw std::runtime_error("failed to create window surface!");
    }
  }

  void createSwapChain()
  {
    int width_buffer_size, height_buffer_size;
    glfwGetFramebufferSize(m_window, &width_buffer_size, &height_buffer_size);

    vkw::SwapChain::createSwapChain
    (
      m_context.physical_device,
      m_context.surface,
      width_buffer_size,
      height_buffer_size,
      &m_context.device,
      &m_context.swap_chain,
      m_context.swap_chain_images,
      m_context.swap_chain_image_format,
      m_context.swap_chain_extent
    );
  }

  void createGraphicsPipeline()
  {
    std::vector<std::string> vert_shader_files = {"vert.spv"};
    std::vector<std::string> frag_shader_files = {"frag.spv"};

    vkw::GraphicsPipeline::createGraphicsPipeline
    (
      &m_context.device,
      &m_context.pipeline_layout,
      vert_shader_files,
      frag_shader_files
    );
  }

  void initVulkan()
  {
    vkw::Instance::createInstance
    (
      &m_context.instance, 
      "Hello Triangle", 
      (int[]){1, 0, 0}, 
      getRequiredExtensions()
    );
    vkw::Validation::setupDebugMessenger
    (
      &m_context.instance, 
      &m_context.debug_messenger
    );
    createSurface();
    vkw::PhysicalDevice::pickPhysicalDevice
    (
      m_context.instance,
      m_context.physical_device,
      m_context.surface
    );
    vkw::LogicalDevice::createLogicalDevice
    (
      m_context.physical_device, 
      m_context.surface, 
      &m_context.device, 
      m_context.graphics_queue,
      m_context.present_queue
    );
    createSwapChain();
    vkw::SwapChain::createImageViews
      (
        &m_context.swap_chain_image_views,
        m_context.swap_chain_images,
        m_context.swap_chain_image_format
      );
    createGraphicsPipeline();
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
    vkw::GraphicsPipeline::destroyGraphicsPipeline();
    vkw::SwapChain::destroyImageViews();
    vkw::SwapChain::destroySwapChain();
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