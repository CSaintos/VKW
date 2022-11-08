// Presentation.hpp
#pragma once

#include <vulkan\vulkan.hpp>

#include <cstdint>

#include <vector>
#include <array>

#include "vkw\Context.hpp"
#include "vkw\Command.hpp"
#include "vkw\VertexBuffer.hpp"

namespace vkw 
{
  class Presentation
  {
  public:
    /**
     * 1st method to call in presentation.
     * - Acquires image from swapchain
     * 
     * @return VkResult for checking if the image
     * has any changes.
    */
    static VkResult acquireNextImage
    (
      Context &context
    );
    /**
     * 2nd method to call in presentation.
     * - Resets old command buffer.
     * - Records new command buffer.
     * - Submits new command buffer to graphics queue.
    */
    static void resubmitCommandBuffer
    (
      Context &context,
      const std::vector<vkw::Vertex> &vertices
    );
    /**
     * 3rd method to call in presentation.
     * - Uses the swapchain and present info
     * to submit the images in the swap chain
     * to the present_queue.
    */
    static VkResult presentQueue
    (
      Context &context
    );
    /**
     * 4th and final method to call in presentation.
     * 
     * updates the current frame to the next frame.
    */
    static void nextFrame
    (
      Context &context,
      const int &flight_frame_count
    );
  private:
    inline static uint32_t m_image_idx = 0;
    inline static std::vector<VkSemaphore> m_signal_semaphores;
  };
}