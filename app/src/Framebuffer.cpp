// Framebuffer.cpp
#include "vkw\Framebuffer.hpp"

void vkw::Framebuffer::createFramebuffers(Context &context)
{
  // link static vars
  m_logical_device = &context.logical_device;
  m_swapchain_framebuffers = &context.swapchain_framebuffers;

  m_swapchain_framebuffers->resize(context.swapchain_image_views.size());

  for (size_t i = 0; i < context.swapchain_image_views.size(); i++)
  {
    std::array<const VkImageView, 1> attachments = 
    {
      context.swapchain_image_views[i]
    };

    VkFramebufferCreateInfo framebuffer_info{};
    framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebuffer_info.renderPass = context.render_pass;
    framebuffer_info.attachmentCount = attachments.size();
    framebuffer_info.pAttachments = attachments.data();
    framebuffer_info.width = context.swapchain_extent.width;
    framebuffer_info.height = context.swapchain_extent.height;
    framebuffer_info.layers = 1;

    if (vkCreateFramebuffer
      (
        *m_logical_device, 
        &framebuffer_info,
        nullptr,
        &(*m_swapchain_framebuffers)[i]
      ) != VK_SUCCESS)
    {
      throw std::runtime_error("failed to create framebuffer!");
    }
  }
}

void vkw::Framebuffer::destroyFramebuffers()
{
  for (VkFramebuffer framebuffer : *m_swapchain_framebuffers)
  {
    vkDestroyFramebuffer(*m_logical_device, framebuffer, nullptr);
  }
}