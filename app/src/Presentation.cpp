// Presentation.cpp
#include "vkw\Presentation.hpp"

VkResult vkw::Presentation::acquireNextImage
(
  Context &context
)
{
  vkWaitForFences
  (
    context.logical_device,
    1,
    &context.in_flight_fences[context.current_frame],
    VK_TRUE,
    UINT64_MAX
  );

  // Acquire an image from the swapchain
  uint32_t image_idx;
  VkResult result = vkAcquireNextImageKHR
  (
    context.logical_device,
    context.swapchain,
    UINT64_MAX,
    context.image_available_semaphores[context.current_frame],
    VK_NULL_HANDLE,
    &image_idx
  );

  return result;
}