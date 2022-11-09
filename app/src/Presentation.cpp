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
  VkResult result = vkAcquireNextImageKHR
  (
    context.logical_device,
    context.swapchain,
    UINT64_MAX,
    context.image_available_semaphores[context.current_frame],
    VK_NULL_HANDLE,
    &m_image_idx
  );

  return result;
}

void vkw::Presentation::resubmitCommandBuffer
(
  Context &context,
  const std::vector<vkw::Vertex> &vertices,
  const std::vector<uint16_t> &indices
)
{
  // Only resets fence if work is submitted
  vkResetFences
  (
    context.logical_device, 
    1, 
    &context.in_flight_fences[context.current_frame]
  );

  // Reset command buffer
  vkResetCommandBuffer(context.command_buffers[context.current_frame], 0);
  // Record command buffer
  Command::recordCommandBuffer
  (
    context.command_buffers[context.current_frame],
    m_image_idx,
    context.render_pass,
    context.swapchain_framebuffers,
    context.swapchain_extent,
    context.graphics_pipeline,
    context.vertex_buffer,
    context.index_buffer,
    vertices,
    indices
  );

  // With a fully recorded command bufer, we can now submit it.
  VkSubmitInfo submit_info{};
  submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  VkSemaphore wait_semaphores[] = 
    {context.image_available_semaphores[context.current_frame]};
  VkPipelineStageFlags wait_stages[] =
    {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  // These three params specify which semaphores to wait on before
  // execution begins and in which stage(s) of the pipeline to wait.
  submit_info.waitSemaphoreCount = 
    sizeof(wait_semaphores) / sizeof(wait_semaphores[0]);
  submit_info.pWaitSemaphores = wait_semaphores;
  submit_info.pWaitDstStageMask = wait_stages;
  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers =
    &context.command_buffers[context.current_frame];
  // The signal semaphore params sepcify which semaphores to signal
  // once the cmd buffers have finished execution.
  m_signal_semaphores =
    {context.render_finished_semaphores[context.current_frame]};
  submit_info.signalSemaphoreCount = m_signal_semaphores.size();
  submit_info.pSignalSemaphores = m_signal_semaphores.data();

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
}

VkResult vkw::Presentation::presentQueue
(
  Context &context
)
{
  /*
  The last step of drawing a frame is submitting the result back to
  the swapchain to show up on screen.
  */
  VkPresentInfoKHR present_info{};
  present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  // First two params specify which semaphores to wait on.
  // We wait on the cmd buffer to finish execution, thus our image
  // is being drawn.
  present_info.waitSemaphoreCount = m_signal_semaphores.size();
  present_info.pWaitSemaphores = m_signal_semaphores.data();
  // Next we specify the swapchains to present images to and the
  // index of the image for each swapchain.
  VkSwapchainKHR swapchains[] = {context.swapchain};
  present_info.swapchainCount = 
    sizeof(swapchains) / sizeof(swapchains[0]);
  present_info.pSwapchains = swapchains;
  present_info.pImageIndices = &m_image_idx;
  // pResults allows to specify an array of VkResult values to check
  // for every individual swapchain if presentation was successful.
  // Only recommended if more than 1 swapchain.
  present_info.pResults = nullptr; // Optional.

  VkResult result = 
    vkQueuePresentKHR(context.present_queue, &present_info);

  return result;
}

void vkw::Presentation::nextFrame
(
  Context &context,
  const int &flight_frame_count
)
{
  context.current_frame = 
    (context.current_frame + 1) &flight_frame_count;
}