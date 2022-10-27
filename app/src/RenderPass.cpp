// RenderPass.cpp
#include "vkw\RenderPass.hpp"

void vkw::RenderPass::createRenderPass
(
  VkDevice *logical_device,
  VkRenderPass *render_pass,
  const VkFormat &swapchain_image_format
)
{
  VkAttachmentDescription color_attachment{};
  color_attachment.format = swapchain_image_format;
  // if multi-sampling, then more than 1.
  color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
  /*
  loadOp and storeOp determine what to do with data in attachment
  before and after rendering.
  loadOp choices:
  - VK_ATTACHMENT_LOAD_OP_LOAD: Preserve the existing contents of 
  the attachment.
  - VK_ATTACHMENT_LOAD_OP_CLEAR: Clear the values to a constant at
  start.
  - VK_ATTACHMENT_LOAD_OP_DONT_CARE: Existing contents are undefined; 
  we don't care about them.
  storeOp choices:
  - VK_ATTACHMENT_STORE_OP_STORE: Rendered contents will be stored
  in memory and can be read later.
  - VK_ATTACHMENT_STORE_OP_DONT_CARE: Contents of the framebuffer
  will be undefined after the rendering operation.
  */
  // since we're interested in seeing the rendered triangle.
  // we're going to store operation here.
  color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  /*
  much like loadOp and storeOp, but for stencil data.
  */
  color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  // initialLayout specifies which layout the image will have b4 the
  // render pass begins.
  color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  // finalLayout specifies the layout to automatically transition to
  // when render pass finishes.
  color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
  /*
  A single render pass may consist of mutliple subpasses.
  subpass - subsequent rendering operation that depends on the 
  contents of framebuffers in previous passes.
  */
  /*
  Every subpass references one or more attachments. 
  */
  // attachment param specifies which attachment to reference by its index.
  // Since our array consists of a single VkAttachmentDescription, its index
  // is 0.
  VkAttachmentReference attachment_ref{};
  attachment_ref.attachment = 0;
  // layout param specifies which layout we would like the attachment to have
  // during a subpass that uses this reference.
  attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass{};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &attachment_ref;

  /*
  subpasses in a render pass automatically take care of image layout transitions.
  The transitions are controlled by subpass dependencies: memory and execution
  dependencies between subpasses.
  */
  // the first two fields specify the indices of the dependecy and the dependent subpass.
  VkSubpassDependency dependency{};
  // refers to implicit subpass before or after the render pass.
  // Since dstSubpass must be higher than srcSubpass, we can specify 
  // VK_SUBPASS_EXTERNAL.
  dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
  // index 0 refers to our subpass, which is the first and only one.
  dependency.dstSubpass = 0;
  /*
  Two dependencies that take care of the transition at the start and end of the render pass.
  But start depedency doesn't occur at the right time, it assumes that the transition
  occurs at the start of the pipeline, but no image is acquired at that point.
  To deal with this, waitStages can be set to:
  - VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT: ensure the render pass doesn't begin until the 
  image is available.
  - VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUT_BIT: waits on the swap chain to finish
  reading the image before we can access it.
  */
  dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  // index 0 refers to our subpass. 
  dependency.srcAccessMask = 0;
  /*
  The next two params specify the operations to wait on and the stages in which these
  operations occurs.
  */
  dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

  VkRenderPassCreateInfo render_pass_info{};
  render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  render_pass_info.attachmentCount = 1;
  render_pass_info.pAttachments = &color_attachment;
  render_pass_info.subpassCount = 1;
  render_pass_info.pSubpasses = &subpass;
  render_pass_info.dependencyCount = 1;
  render_pass_info.pDependencies = &dependency;

  if (vkCreateRenderPass
    (
      *logical_device,
      &render_pass_info,
      nullptr,
      render_pass
    ) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create render pass!");
  }

  m_logical_device = logical_device;
  m_render_pass = render_pass;
}

void vkw::RenderPass::destroyRenderPass()
{
  vkDestroyRenderPass(*m_logical_device, *m_render_pass, nullptr);
}