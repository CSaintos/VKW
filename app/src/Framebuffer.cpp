// Framebuffer.cpp
#include "vkw\Framebuffer.hpp"

void vkw::Framebuffer::createFramebuffers
(
  VkDevice *logical_device,
  std::vector<VkFramebuffer> *swapchain_framebuffers,
  const std::vector<VkImageView> &swapchain_image_views,
  const VkRenderPass &render_pass,
  const VkExtent2D &swapchain_extent
)
{
  swapchain_framebuffers->resize(swapchain_image_views.size());

  for (size_t i = 0; i < swapchain_image_views.size(); i++)
  {
    std::array<const VkImageView, 1> attachments = 
    {
      swapchain_image_views[i]
    };

    VkFramebufferCreateInfo framebuffer_info{};
    framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebuffer_info.renderPass = render_pass;
    framebuffer_info.attachmentCount = attachments.size();
    framebuffer_info.pAttachments = attachments.data();
    framebuffer_info.width = swapchain_extent.width;
    framebuffer_info.height = swapchain_extent.height;
    framebuffer_info.layers = 1;

    if (vkCreateFramebuffer
      (
        *logical_device, 
        &framebuffer_info,
        nullptr,
        &(*swapchain_framebuffers)[i]
      ) != VK_SUCCESS)
    {
      throw std::runtime_error("failed to create framebuffer!");
    }
  }

  m_logical_device = logical_device;
  m_swapchain_framebuffers = swapchain_framebuffers;
}

void vkw::Framebuffer::destroyFramebuffers()
{
  for (VkFramebuffer framebuffer : *m_swapchain_framebuffers)
  {
    vkDestroyFramebuffer(*m_logical_device, framebuffer, nullptr);
  }
}