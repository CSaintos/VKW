// Command.hpp
#pragma once

#include <vulkan\vulkan.hpp>

#include <cstdint>

#include <vector>

#include "vkw\QueueFamilyIndices.hpp"

namespace vkw
{
  /**
   * Commands in Vulkan, like drawing operations and memory transfers,
   * are not executed directly using function calls. You have to record
   * all of the operations you want to perform in command buffer objects.
   * 
   * This allows command recording to happen in multiple threads if needed.
  */
  class Command
  {
  public:
    /**
     * @brief createCommandPool()
     * 
     * Before creating command buffers, we must create a command pool.
     * Command pool - manages the memory that us used to store the buffers
     * and command buffers that are allocated from them.
     * 
     * @param logical_device from which we get the device queues.
     * @param command_pool
     * @param physical_device needed to retrieve queue family indices
     * @param surface needed to retrieve queue family indices
    */
    static void createCommandPool
    (
      const VkDevice *logical_device,
      VkCommandPool *command_pool,
      const VkPhysicalDevice &physical_device,
      const VkSurfaceKHR &surface
    );
    static void destroyCommandPool();
    static void createCommandBuffer
    (
      VkCommandBuffer *command_buffer
    );
    /**
     * recordCommandBuffer()
     * 
     * @brief writes commands we want to execute into a cmd buffer.
     * 
     * @param command_buffer the command buffer to write to.
     * @param image_idx index of the current swapchain image we want to write to.
    */
    static void recordCommandBuffer
    (
      VkCommandBuffer command_buffer,
      uint32_t image_idx,
      const VkRenderPass &render_pass,
      const std::vector<VkFramebuffer> &swap_chain_framebuffers,
      const VkExtent2D &swap_chain_extent,
      const VkPipeline &graphics_pipeline
    );
  private:
    inline static const VkDevice *m_logical_device = nullptr;
    inline static VkCommandPool *m_command_pool = nullptr;
  };
}