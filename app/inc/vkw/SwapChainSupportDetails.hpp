// SwapChainSupportDetails.hpp
#pragma once

#include <vulkan/vulkan.hpp>

#include <vector>

namespace vkw
{
  struct SwapChainSupportDetails
  {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> present_modes;
  };
}