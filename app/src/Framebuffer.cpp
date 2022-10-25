// Framebuffer.cpp
#include "vkw\Framebuffer.hpp"

void vkw::Framebuffer::createFramebuffers
(
  const VkDevice *logical_device,
  std::vector<VkFramebuffer> *swap_chain_framebuffers,
  const std::vector<VkImageView> &swap_chain_image_views,
  const VkRenderPass &render_pass,
  const VkExtent2D &swap_chain_extent
)
{
  swap_chain_framebuffers->resize(swap_chain_image_views.size());

  for (size_t i = 0; i < swap_chain_image_views.size(); i++)
  {
    std::array<VkImageView, 1> attachments = 
    {
      swap_chain_image_views[i]
    };

    VkFramebufferCreateInfo framebuffer_info{};
    framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebuffer_info.renderPass = render_pass;
    framebuffer_info.attachmentCount = attachments.size();
    framebuffer_info.pAttachments = attachments.data();
    framebuffer_info.width = swap_chain_extent.width;
    framebuffer_info.height = swap_chain_extent.height;
    framebuffer_info.layers = 1;

    if (vkCreateFramebuffer
      (
        *logical_device, 
        &framebuffer_info,
        nullptr,
        &(*swap_chain_framebuffers)[i]
      ) != VK_SUCCESS)
    {
      throw std::runtime_error("failed to create framebuffer!");
    }
  }

  m_logical_device = logical_device;
  m_swap_chain_framebuffers = swap_chain_framebuffers;
}

void vkw::Framebuffer::destroyFramebuffers()
{
  for (VkFramebuffer framebuffer : *m_swap_chain_framebuffers)
  {
    vkDestroyFramebuffer(*m_logical_device, framebuffer, nullptr);
  }
}