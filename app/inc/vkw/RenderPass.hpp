// RenderPass.hpp
#pragma once

#include <vulkan\vulkan.hpp>

#include "vkw\Context.hpp"

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
    static void createRenderPass(Context &context);
    static void destroyRenderPass();
  private:
    RenderPass();

    inline static VkDevice *m_logical_device = nullptr;
    inline static VkRenderPass *m_render_pass = nullptr;
  };
}