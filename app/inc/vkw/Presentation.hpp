// Presentation.hpp
#pragma once

#include <vulkan\vulkan.hpp>

#include <cstdint>

#include <vector>

#include "vkw\Command.hpp"
#include "vkw\Swapchain.hpp"

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
      const int &flight_frame_count,
      std::array<int, 2> (*updateFramebufferSize)()
    );
    static void acquireNextImage
    (
      VkDevice &logical_device,
      std::vector<VkFence> &in_flight_fences,
      uint32_t &current_frame,
      VkSwapchainKHR &swapchain,
      std::vector<VkSemaphore> &image_available_semaphores,
      std::vector<VkFramebuffer> &swapchain_framebuffers,
      VkRenderPass &render_pass,
      uint32_t &current_image_idx,
      std::array<int, 2> (*updateFramebufferSize)()
    );
    static void recordCmdBuffer
    (
      std::vector<VkCommandBuffer> &command_buffers,
      uint32_t &current_frame,
      uint32_t &current_image_idx,
      VkRenderPass &render_pass,
      std::vector<VkFramebuffer> &swapchain_framebuffers,
      VkExtent2D &swapchain_extent,
      VkPipeline &graphics_pipeline
    );
  };
}