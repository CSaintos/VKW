// Synchronization.hpp
#include "vkw\Synchronization.hpp"

void vkw::Synchronization::createSyncObjects
(
  Context &context,
  const int *flight_frame_count
)
{
  // link static vars
  m_logical_device = &context.logical_device;
  m_image_available_semaphores = &context.image_available_semaphores;
  m_render_finished_semaphores = &context.render_finished_semaphores;
  m_in_flight_fences = &context.in_flight_fences;
  m_flight_frame_count = flight_frame_count;

  m_image_available_semaphores->resize(*flight_frame_count);
  m_render_finished_semaphores->resize(*flight_frame_count);
  m_in_flight_fences->resize(*flight_frame_count);

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
        *m_logical_device, 
        &semaphore_info, 
        nullptr, 
        &(*m_image_available_semaphores)[i]
      ) != VK_SUCCESS ||
      vkCreateSemaphore
      (
        *m_logical_device,
        &semaphore_info,
        nullptr,
        &(*m_render_finished_semaphores)[i]
      ) != VK_SUCCESS ||
      vkCreateFence
      (
        *m_logical_device,
        &fence_info,
        nullptr,
        &(*m_in_flight_fences)[i]
      ) != VK_SUCCESS
    )
    {
      throw std::runtime_error("failed to create semaphores!");
    }
  }
}

void vkw::Synchronization::destroySyncObjects()
{
  for (size_t i = 0; i < *m_flight_frame_count; i++)
  {
    vkDestroySemaphore(*m_logical_device, (*m_image_available_semaphores)[i], nullptr);
    vkDestroySemaphore(*m_logical_device, (*m_render_finished_semaphores)[i], nullptr);
    vkDestroyFence(*m_logical_device, (*m_in_flight_fences)[i], nullptr);
  }
}