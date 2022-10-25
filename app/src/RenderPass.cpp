// RenderPass.cpp
#include "vkw\RenderPass.hpp"

void vkw::RenderPass::createRenderPass
(
  const VkDevice *logical_device,
  VkRenderPass *render_pass,
  const VkFormat &swap_chain_image_format
)
{
  m_color_attachment.format = swap_chain_image_format;
  // if multi-sampling, then more than 1.
  m_color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
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
  m_color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  m_color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  /*
  much like loadOp and storeOp, but for stencil data.
  */
  m_color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  m_color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  // initialLayout specifies which layout the image will have b4 the
  // render pass begins.
  m_color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  // finalLayout specifies the layout to automatically transition to
  // when render pass finishes.
  m_color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
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
  m_color_attachment_ref.attachment = 0;
  // layout param specifies which layout we would like the attachment to have
  // during a subpass that uses this reference.
  m_color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  m_subpass_desc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  m_subpass_desc.colorAttachmentCount = 1;
  m_subpass_desc.pColorAttachments = &m_color_attachment_ref;

  m_create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  m_create_info.attachmentCount = 1;
  m_create_info.pAttachments = &m_color_attachment;
  m_create_info.subpassCount = 1;
  m_create_info.pSubpasses = &m_subpass_desc;

  if (vkCreateRenderPass
    (
      *logical_device,
      &m_create_info,
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