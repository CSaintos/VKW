// Presentation.hpp
#pragma once

#include <vulkan\vulkan.hpp>

#include <vector>
#include <array>

#include "vkw\Context.hpp"

namespace vkw 
{
  class Presentation
  {
  public:
    /**
     * 1st method to call in presentation.
     * - Acquires image from swapchain
     * 
     * @return VkResult for checking if the image
     * has any changes.
    */
    static VkResult acquireNextImage
    (
      Context &context
    );
  };
}