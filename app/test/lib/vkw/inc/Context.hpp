// Context.hpp
#pragma once

#include <vulkan\vulkan.hpp>

#include <cstdint>

#include <vector>

namespace vkw
{
  struct Context
  {
    VkInstance instance;
    VkPhysicalDevice physical_device = VK_NULL_HANDLE;
    VkDevice device;
    VkQueue graphics_queue;
    VkQueue present_queue;
    VkSurfaceKHR surface;
    VkSwapchainKHR swap_chain;
    std::vector<VkImage> swap_chain_images;
    VkRenderPass render_pass;
    VkPipelineLayout pipeline_layout;
    VkPipeline graphics_pipeline;
    std::vector<VkFramebuffer> swap_chain_framebuffers;
    VkCommandPool command_pool;
    std::vector<VkCommandBuffer> command_buffers;
    std::vector<VkSemaphore> image_available_semaphores;
    std::vector<VkSemaphore> render_finished_semaphores;
    std::vector<VkFence> in_flight_fences;
    uint32_t current_frame = 0;
    bool framebuffer_resized = false;
  };
}
