// RenderPass.hpp
#pragma once

#include <vulkan\vulkan.hpp>

namespace vkw
{
  /**
   * A render pass object is a wrapper for:
   * - Framebuffer attachments used while rendering
   * - specifications for:
   *   - # of color and depth buffers
   *   - their samples
   *   - how their contents should be handled during rendering 
   *  operations.
  */
  class RenderPass
  {
  public:
    static void createRenderPass
    (
      const VkDevice *logical_device,
      VkRenderPass *render_pass,
      const VkFormat &swap_chain_image_format
    );
    static void destroyRenderPass();
  private:
    inline static VkAttachmentDescription m_color_attachment{};
    inline static VkAttachmentReference m_color_attachment_ref{};
    inline static VkSubpassDescription m_subpass_desc{};
    inline static VkRenderPassCreateInfo m_create_info{};

    inline static const VkDevice *m_logical_device = nullptr;
    inline static VkRenderPass *m_render_pass = nullptr;
  };
}