// Presentation.cpp
#include "Presentation.hpp"

void Presentation::drawFrame
(
  vkw::Context &context,
  const int &flight_frame_count,
  GLFWwindow *window,
  std::vector<vkw::Vertex> &vertices
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

  if (result == VK_ERROR_OUT_OF_DATE_KHR)
  {
    std::array<int, 2> buffer_size = 
      updateFramebufferSize(window);
    vkw::Swapchain::recreateSwapchain
    (
      &context.swapchain_framebuffers,
      context.render_pass,
      buffer_size[0],
      buffer_size[1]
    );
    return;
  }
  else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
  {
    throw std::runtime_error("failed to acquire swapchain image!");
  }

  // Only reset fence if work is submitted
  vkResetFences(context.logical_device, 1, &context.in_flight_fences[context.current_frame]);

  // Record the command buffer
  vkResetCommandBuffer(context.command_buffers[context.current_frame], 0);
  vkw::Command::recordCommandBuffer
  (
    context.command_buffers[context.current_frame],
    image_idx,
    context.render_pass,
    context.swapchain_framebuffers,
    context.swapchain_extent,
    context.graphics_pipeline,
    context.vertex_buffer,
    vertices
  );

  // With a fully recorded command buffer, we can now submit it.
  VkSubmitInfo submit_info{};
  submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  VkSemaphore wait_semaphores[] = 
    {context.image_available_semaphores[context.current_frame]};
  VkPipelineStageFlags wait_stages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  // These three params specify which semaphores to wait on before execution begins
  // and in which stage(s) of the pipeline to wait.
  submit_info.waitSemaphoreCount = sizeof(wait_semaphores) / sizeof(wait_semaphores[0]);
  submit_info.pWaitSemaphores = wait_semaphores;
  submit_info.pWaitDstStageMask = wait_stages;
  // The next two params specify which command buffers to submit for execution.
  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers = &context.command_buffers[context.current_frame];
  // The signal semaphore params specify which semaphores to signal once
  // the cmd buffers have finished execution.
  VkSemaphore signal_semaphores[] = {context.render_finished_semaphores[context.current_frame]};
  submit_info.signalSemaphoreCount = sizeof(signal_semaphores) / sizeof(signal_semaphores[0]);
  submit_info.pSignalSemaphores = signal_semaphores;

  if 
  (
    vkQueueSubmit
    (
      context.graphics_queue,
      1,
      &submit_info,
      context.in_flight_fences[context.current_frame]
    ) != VK_SUCCESS
  )
  {
    throw std::runtime_error("failed to submit draw command buffer!");
  }

  /*
  The last step of drawing a frame is submitting the result back to the swapchain
  to show up on screen.
  */
  VkPresentInfoKHR present_info{};
  present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  // First two params specify which semaphores to wait on.
  // We wait on the cmd buffer tof insih execution, thus our image is being drawn.
  present_info.waitSemaphoreCount = 
    sizeof(signal_semaphores) / sizeof(signal_semaphores[0]);
  present_info.pWaitSemaphores = signal_semaphores;
  // Next we specify the swapchains to present images to and the index of the image
  // for each swapchain.
  VkSwapchainKHR swapchains[] = {context.swapchain};
  present_info.swapchainCount = sizeof(swapchains) / sizeof(swapchains[0]);
  present_info.pSwapchains = swapchains;
  present_info.pImageIndices = &image_idx;
  // pReseults allows to specify an array of VkResult values to check for every individual
  // swapchain if presentation was successful. Only recommended if more than 1 swapchain.
  present_info.pResults = nullptr; // Optional

  result = vkQueuePresentKHR(context.present_queue, &present_info);

  if (result == VK_ERROR_OUT_OF_DATE_KHR ||
    result == VK_SUBOPTIMAL_KHR||
    context.framebuffer_resized)
  {
    context.framebuffer_resized = false;
    std::array<int, 2> buffer_size = 
      updateFramebufferSize(window);
    vkw::Swapchain::recreateSwapchain
    (
      &context.swapchain_framebuffers,
      context.render_pass,
      buffer_size[0],
      buffer_size[1]
    );
  }
  else if (result != VK_SUCCESS)
  {
    throw std::runtime_error("failed to present swapchain image!");
  }

  context.current_frame = (context.current_frame + 1) & flight_frame_count;
}

/**
 * Gets the framebuffer size for swapchain creation.
 * 
 * It also checks if the window is
 * minimized. If so, it waits till the window is back to 
 * normal.
*/
std::array<int, 2> Presentation::updateFramebufferSize
(
  GLFWwindow *window
)
{
  int width_buffer_size = 0, height_buffer_size = 0;
  glfwGetFramebufferSize
  (
    window, 
    &width_buffer_size, 
    &height_buffer_size
  );
  while 
  (
    width_buffer_size == 0 || 
    height_buffer_size == 0
  )
  {
    glfwGetFramebufferSize
    (
      window, 
      &width_buffer_size, 
      &height_buffer_size
    );
    glfwWaitEvents();
  }
  return {width_buffer_size, height_buffer_size};
}