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

  const int MAX_FRAMES_IN_FLIGHT = 2;

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

    vkw::Swapchain::createSwapchain
    (
      m_context.physical_device,
      m_context.surface,
      width_buffer_size,
      height_buffer_size,
      &m_context.logical_device,
      &m_context.swapchain,
      m_context.swapchain_images,
      m_context.swapchain_image_format,
      m_context.swapchain_extent
    );
  }

  void createGraphicsPipeline()
  {
    std::vector<std::string> vert_shader_files = {"vert.spv"};
    std::vector<std::string> frag_shader_files = {"frag.spv"};

    vkw::GraphicsPipeline::createGraphicsPipeline
    (
      &m_context.logical_device,
      &m_context.render_pass,
      &m_context.pipeline_layout,
      &m_context.graphics_pipeline,
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
      &m_context.logical_device, 
      m_context.graphics_queue,
      m_context.present_queue
    );
    createSwapChain();
    vkw::Swapchain::createImageViews
    (
      &m_context.swapchain_image_views,
      m_context.swapchain_images,
      m_context.swapchain_image_format
    );
    vkw::RenderPass::createRenderPass
    (
      &m_context.logical_device,
      &m_context.render_pass,
      m_context.swapchain_image_format
    );
    createGraphicsPipeline();
    vkw::Framebuffer::createFramebuffers
    (
      &m_context.logical_device,
      &m_context.swapchain_framebuffers,
      m_context.swapchain_image_views,
      m_context.render_pass,
      m_context.swapchain_extent
    );
    vkw::Command::createCommandPool
    (
      &m_context.logical_device,
      &m_context.command_pool,
      m_context.physical_device,
      m_context.surface
    );
    vkw::Command::createCommandBuffers
    (
      m_context.command_buffers,
      MAX_FRAMES_IN_FLIGHT
    );
    vkw::Synchronization::createSyncObjects
    (
      &m_context.logical_device,
      &m_context.image_available_semaphores,
      &m_context.render_finished_semaphores,
      &m_context.in_flight_fences,
      &MAX_FRAMES_IN_FLIGHT
    );
  }

  void mainLoop()
  {
    while (!glfwWindowShouldClose(m_window))
    {
      glfwPollEvents();
      vkw::Presentation::drawFrame
      (
        m_context.logical_device,
        m_context.in_flight_fences,
        m_context.swapchain,
        m_context.image_available_semaphores,
        m_context.render_finished_semaphores,
        m_context.command_buffers,
        m_context.render_pass,
        m_context.swapchain_framebuffers,
        m_context.swapchain_extent,
        m_context.graphics_pipeline,
        m_context.graphics_queue,
        m_context.present_queue,
        m_context.current_frame,
        m_context.framebuffer_resized,
        MAX_FRAMES_IN_FLIGHT        
      );
    }

    vkDeviceWaitIdle(m_context.logical_device);
  }

  void cleanup()
  {
    vkw::Synchronization::destroySyncObjects();
    vkw::Command::destroyCommandPool();
    vkw::Framebuffer::destroyFramebuffers();
    vkw::GraphicsPipeline::destroyGraphicsPipeline();
    vkw::RenderPass::destroyRenderPass();
    vkw::Swapchain::destroyImageViews();
    vkw::Swapchain::destroySwapchain();
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