// Command.cpp
#include "vkw\Command.hpp"

void vkw::Command::createCommandPool(Context &context)
{
  // link static vars
  m_logical_device = &context.logical_device;
  m_command_pool = &context.command_pool;

  QueueFamilyIndices queue_family_indices = 
    QueueFamilyIndices::findQueueFamilies(context.physical_device, context.surface);

  VkCommandPoolCreateInfo pool_info{};
  pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  /*
  Two possible flags for cmd pools:
  - VK_COMMAND_POOL_CREATE_TRANSIENT_BIT: Hint that cmd buffers are rerecorded
  with new commands very often (may change memory allocation behavior).
  - VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT: Allow cmd buffers to be
  rerecorded individually, without this flag they all have to be reset together.
  */
  pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  pool_info.queueFamilyIndex = queue_family_indices.graphics_family.value();

  /*
  Command buffers are executed by submitting them on one of the device queues.
  Each command pool can only allocate cmd buffers that are submitted on a single
  type of queue. We used the graphics queue family.
  */
  if (vkCreateCommandPool
    (
      *m_logical_device,
      &pool_info,
      nullptr,
      m_command_pool
    ) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create command pool!");
  }
}

void vkw::Command::destroyCommandPool()
{
  vkDestroyCommandPool(*m_logical_device, *m_command_pool, nullptr);
}

void vkw::Command::createCommandBuffers
(
  std::vector<VkCommandBuffer> &command_buffers,
  const int &flight_frame_count
)
{
  command_buffers.resize(flight_frame_count);

  VkCommandBufferAllocateInfo alloc_info{};
  alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  alloc_info.commandPool = *m_command_pool;
  /*
  level param specifies if allocated cmd buffers are primary or secondary
  cmd buffers.
  - VK_COMMAND_BUFFER_LEVEL_PRIMARY: can be submitted to a queue for 
  execution, but can't be called from other cmd buffers.
  - VK_COMMAND_BUFFER_LEVEL_SECONDARY: can't be sumitted directly, but can
  be called from primary cmd buffers.
  */
  alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  alloc_info.commandBufferCount = (uint32_t) flight_frame_count;

  if (vkAllocateCommandBuffers
    (
      *m_logical_device,
      &alloc_info,
      command_buffers.data()
    ) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to allocate command buffers!");
  }
}

void vkw::Command::recordCommandBuffer
(
  VkCommandBuffer command_buffer,
  uint32_t image_idx,
  VkRenderPass &render_pass,
  std::vector<VkFramebuffer> &swapchain_framebuffers,
  VkExtent2D &swap_chain_extent,
  VkPipeline &graphics_pipeline,
  VkBuffer &vertex_buffer,
  const std::vector<Vertex> &vertices
)
{
  VkCommandBufferBeginInfo begin_info{};
  begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  /*
  flags param specifies how we're going to use the cmd buffer. values:
  - VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT: the cmd buffer will be 
  rerecorded right after executing it once.
  - VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT: this is a 
  secondary cmd buffer that will be entirely within a single render pass.
  - VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT: the cmd buffer can be 
  resubmitted while it is also already pending execution.
  */
  begin_info.flags = 0; // Optional
  // pInheritanceInfo param is only relevant for secondary cmd buffers.
  // says which state to inherit from the calling primary cmd buffers.
  begin_info.pInheritanceInfo = nullptr; // Optional

  /*
  If the cmd buffer was already recorded once, then a call to 
  vkBeginCommandBuffer will implicitly reset it.
  */
  if (vkBeginCommandBuffer(command_buffer, &begin_info) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to begin recording command buffer!");
  }

  VkRenderPassBeginInfo render_pass_info{};
  render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  render_pass_info.renderPass = render_pass;
  // binds the framebuffer for the swapchain image we want to draw to.
  render_pass_info.framebuffer = swapchain_framebuffers[image_idx];
  render_pass_info.renderArea.offset = {0, 0};
  render_pass_info.renderArea.extent = swap_chain_extent;
  /*
  The next two params define the clear values to use for 
  VK_ATTACHMENT_LOAD_OP_CLEAR, which we used as load operation
  for the color attachment.
  */
  // The clear_color is simply black with 100% opacity.
  VkClearValue clear_color = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
  render_pass_info.clearValueCount = 1;
  render_pass_info.pClearValues = &clear_color;

  /*
  The render pass can begin. All functions that record cmds are recognized 
  by their `vkCmd` prefix. They all return void, so no error handling 
  until we've finished recording.

  command_buffer param, is the cmd buffer to record the cmd to .
  render_pass_info param, specifies details of the render pass.
  the final param controls how the drawing cmd within the render pass
  will be provided. two values:
  - VK_SUBPASS_CONTENTS_INLINE: the render psas cmds will be embedded
  in the primary cmd buffer itself and no secondary cmd buffers
  will be executed.
  - VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS: the render pass
  cmds will be executed from secondary cmd buffers.
  */
  vkCmdBeginRenderPass(command_buffer, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(
      command_buffer, 
      VK_PIPELINE_BIND_POINT_GRAPHICS, 
      graphics_pipeline
    );

    /*
    Since we specied that the viewport and scissor state for the pipeline be
    dynamic, we need to set them in the cmd buffer before issuing the 
    draw cmd.
    */
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(swap_chain_extent.width);
    viewport.height = static_cast<float>(swap_chain_extent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(command_buffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = swap_chain_extent;
    vkCmdSetScissor(command_buffer, 0, 1, &scissor);

    VkBuffer vertex_buffers[] = {vertex_buffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(command_buffer, 0, 1, vertex_buffers, offsets);

    /*
    vkCmdDraw()
    - vertexCount: we don't have a vertex buffer, we specify 3 vertices to draw.
    - instanceCount: Used for instanced rendering, use 1 if you're not doing that.
    - firstVertex: Used as an offset to the vertex buffer, determines the 
    lowest value of gl_VertexIndex.
    - firstInstance: Used as an offset for instanced rendering, defines the lowest
    value of gl_InstanceIndex.
    */
    vkCmdDraw(command_buffer, static_cast<uint32_t>(vertices.size()), 1, 0, 0);
  
  // end render pass for recording cmd buffer
  vkCmdEndRenderPass(command_buffer);
  
  // finish recording cmd buffer
  if (vkEndCommandBuffer(command_buffer) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to record command buffer!");
  }
}