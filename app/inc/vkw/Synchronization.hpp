// Synchronization.hpp
#pragma once

#include <vulkan\vulkan.hpp>

namespace vkw
{
  class Synchronization
  {
  public:
    static void createSyncObjects
    (
      VkDevice *logical_device,
      std::vector<VkSemaphore> *image_available_semaphores,
      std::vector<VkSemaphore> *render_finished_semaphores,
      std::vector<VkFence> *in_flight_fences,
      const int *flight_frame_count
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
