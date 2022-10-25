// Framebuffer.hpp
#pragma once

#include <vulkan\vulkan.hpp>

#include <cstdint>

#include <vector>
#include <array>

namespace vkw
{
  /**
   * Attachments specified during render pass creation are
   * bound by wrapping them into a VkFramebuffer.
   * A framebuffer object references all of VkImageView objects
   * that represent the attachments.
   * The image that we have to use for the attachment
   * depends on which image the swap chain returns when we retrieve
   * one for presentation.
   * Therefore, we create a framebuffer for all the images in the
   * swap chain and use the one that corresponds to the retrieved image
   * during drawing time.
  */
  class Framebuffer
  {
  public:
    static void createFramebuffers
    (
      const VkDevice *logical_device,
      std::vector<VkFramebuffer> *swap_chain_framebuffers,
      const std::vector<VkImageView> &swap_chain_image_views,
      const VkRenderPass &render_pass,
      const VkExtent2D &swap_chain_extent
    );
    static void destroyFramebuffers();
  private:
    inline static const VkDevice *m_logical_device = nullptr;
    inline static std::vector<VkFramebuffer> *m_swap_chain_framebuffers = nullptr;
  };
}