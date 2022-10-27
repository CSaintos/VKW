// Presentation.cpp
#include "vkw\Presentation.hpp"

void vkw::Presentation::drawFrame
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
  const int &flight_frame_count
)
{
  vkWaitForFences
  (
    logical_device, 
    1, 
    &in_flight_fences[current_frame], 
    VK_TRUE, 
    UINT64_MAX
  );

  // Acquire an image from the swapchain
  uint32_t image_idx;
  VkResult result = vkAcquireNextImageKHR
  (
    logical_device, 
    swapchain,
    UINT64_MAX,
    image_available_semaphores[current_frame],
    VK_NULL_HANDLE,
    &image_idx 
  );
  
  if (result == VK_ERROR_OUT_OF_DATE_KHR)
  {
    // recreateSwapChain();
    return;
  }
  else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
  {
    throw std::runtime_error("failed to acquire swapchain image!");
  }
  
  vkResetFences(logical_device, 1, &in_flight_fences[current_frame]);

  // Record the command buffer
  vkResetCommandBuffer(command_buffers[current_frame], 0);
  Command::recordCommandBuffer
  (
    command_buffers[current_frame],
    image_idx,
    render_pass,
    swapchain_framebuffers,
    swapchain_extent,
    graphics_pipeline
  );

  // With a fully recorded command buffer, we can now submit it.
  VkSubmitInfo submit_info{};
  submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  VkSemaphore wait_semaphores[] = {image_available_semaphores[current_frame]};
  VkPipelineStageFlags wait_stages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  // These three params specify which semaphores to wait on before execution begins
  // and in which stage(s) of the pipeline to wait.
  submit_info.waitSemaphoreCount = sizeof(wait_semaphores) / sizeof(wait_semaphores[0]);
  submit_info.pWaitSemaphores = wait_semaphores;
  submit_info.pWaitDstStageMask = wait_stages;
  // The next two params specify whcih command buffers to submit for execution.
  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers = &command_buffers[current_frame];
  // The signal semaphore params specify which semaphores to signal once
  // the cmd buffers have finished execution.
  VkSemaphore signal_semaphores[] = {render_finished_semaphores[current_frame]};
  submit_info.signalSemaphoreCount = sizeof(signal_semaphores) / sizeof(signal_semaphores[0]);
  submit_info.pSignalSemaphores = signal_semaphores;
  
  if (vkQueueSubmit
    (
      graphics_queue,
      1,
      &submit_info,
      in_flight_fences[current_frame]
    ) != VK_SUCCESS)
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
  // We wait on the cmd buffer to finish execution, thus our image is being drawn.
  present_info.waitSemaphoreCount = sizeof(signal_semaphores) / sizeof(signal_semaphores[0]);
  present_info.pWaitSemaphores = signal_semaphores;
  // Next we specify the swapchains to present images to and the index of the image
  // for each swapchain.
  VkSwapchainKHR swapchains[] = {swapchain};
  present_info.swapchainCount = sizeof(swapchains) / sizeof(swapchains[0]);
  present_info.pSwapchains = swapchains;
  present_info.pImageIndices = &image_idx;
  // pReseults allows to specify an array of VkResult values to check for every individual
  // swapchain if presentation was successful. Only recommended if more than 1 swapchain.
  present_info.pResults = nullptr; // Optional

  result = vkQueuePresentKHR(present_queue, &present_info);

  if (result == VK_ERROR_OUT_OF_DATE_KHR ||
    result == VK_SUBOPTIMAL_KHR ||
    framebuffer_resized)
  {
    framebuffer_resized = false;
    // recreateSwapchain(); 
  }
  else if (result != VK_SUCCESS)
  {
    throw std::runtime_error("failed to present swapchain image!");
  }

  current_frame = (current_frame + 1) % flight_frame_count; 
}