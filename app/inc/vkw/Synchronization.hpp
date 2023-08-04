// Synchronization.hpp
#pragma once

#include <vulkan\vulkan.hpp>

#include "vkw\Context.hpp"

namespace vkw
{
  class Synchronization
  {
  public:
    static void createSyncObjects
    (
      Context &context,
      const int &flight_frame_count
    );
    static void destroySyncObjects();
  private:
    inline static VkDevice *m_logical_device = nullptr;
    inline static std::vector<VkSemaphore> *m_image_available_semaphores = nullptr;
    inline static std::vector<VkSemaphore> *m_render_finished_semaphores = nullptr;
    inline static std::vector<VkFence> *m_in_flight_fences = nullptr;
    inline static const int *m_flight_frame_count = nullptr;
  };
}
