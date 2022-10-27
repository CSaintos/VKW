// Presentation.hpp
#pragma once

#include <vulkan\vulkan.hpp>

#include <cstdint>

#include <vector>

#include "vkw\Command.hpp"

namespace vkw
{
  class Presentation
  {
  public:
    static void drawFrame
    (
      VkDevice &logical_device,
      std::vector<VkFence> &in_flight_fences,
      VkSwapchainKHR &swapchain,
      std::vector<VkSemaphore> &image_available_semaphores,
      const std::vector<VkSemaphore> &render_finished_semaphores,
      std::vector<VkCommandBuffer> &command_buffers,
      VkRenderPass &render_pass,
      std::vector<VkFramebuffer> &swapchain_framebuffers,
      VkExtent2D &swapchain_extent,
      VkPipeline &graphics_pipeline,
      VkQueue &graphics_queue,
      VkQueue &present_queue,
      uint32_t &current_frame,
      bool &framebuffer_resized,
      const int &flight_frame_count
    );
  };
}