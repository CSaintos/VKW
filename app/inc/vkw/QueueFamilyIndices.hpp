// QueueFamilyIndices.hpp
#pragma once

#include <vulkan\vulkan.hpp>

#include <cstdint>

#include <optional>

namespace vkw
{
  struct QueueFamilyIndices
  {
    std::optional<uint32_t> graphics_family;
    std::optional<uint32_t> present_family;

    bool isComplete()
    {
      return graphics_family.has_value() && present_family.has_value();
    }

    static QueueFamilyIndices findQueueFamilies
    (
      const VkPhysicalDevice &device,
      const VkSurfaceKHR &surface
    );
  };
}