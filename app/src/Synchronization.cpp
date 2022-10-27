// Synchronization.hpp
#include "vkw\Synchronization.hpp"

void vkw::Synchronization::createSyncObjects
(
  VkDevice *logical_device,
  std::vector<VkSemaphore> *image_available_semaphores,
  std::vector<VkSemaphore> *render_finished_semaphores,
  std::vector<VkFence> *in_flight_fences,
  const int *flight_frame_count
)
{
  image_available_semaphores->resize(*flight_frame_count);
  render_finished_semaphores->resize(*flight_frame_count);
  in_flight_fences->resize(*flight_frame_count);

  VkSemaphoreCreateInfo semaphore_info{};
  semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VkFenceCreateInfo fence_info{};
  fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  for (size_t i = 0; i < *flight_frame_count; i++)
  {
    if 
    (
      vkCreateSemaphore
      (
        *logical_device, 
        &semaphore_info, 
        nullptr, 
        &(*image_available_semaphores)[i]
      ) != VK_SUCCESS ||
      vkCreateSemaphore
      (
        *logical_device,
        &semaphore_info,
        nullptr,
        &(*render_finished_semaphores)[i]
      ) != VK_SUCCESS ||
      vkCreateFence
      (
        *logical_device,
        &fence_info,
        nullptr,
        &(*in_flight_fences)[i]
      ) != VK_SUCCESS
    )
    {
      throw std::runtime_error("failed to create semaphores!");
    }
  }

  m_logical_device = logical_device;
  m_image_available_semaphores = image_available_semaphores;
  m_render_finished_semaphores = render_finished_semaphores;
  m_in_flight_fences = in_flight_fences;
  m_flight_frame_count = flight_frame_count;
}

void vkw::Synchronization::destroySyncObjects()
{
  for (size_t i = 0; i < *m_flight_frame_count; i++)
  {
    vkDestroySemaphore(*m_logical_device, (*m_image_available_semaphores)[i], nullptr);
    vkDestroySemaphore(*m_logical_device, (*m_image_available_semaphores)[i], nullptr);
    vkDestroyFence(*m_logical_device, (*m_in_flight_fences)[i], nullptr);
  }
}