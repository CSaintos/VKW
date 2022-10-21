// PhysicalDevice.hpp
#pragma once

#include <vulkan\vulkan.hpp>

#include <cstdint>

#include <vector>
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
  };

  class PhysicalDevice
  {
  public:
    static void pickPhysicalDevice
    (
      const VkInstance &vk_instance, 
      VkPhysicalDevice &physical_device,
      const VkSurfaceKHR &surface
    );
    static QueueFamilyIndices findQueueFamilies
    (
      const VkPhysicalDevice &device,
      const VkSurfaceKHR &surface
    );
  private:
    static bool isDeviceSuitable
    (
      const VkPhysicalDevice &device,
      const VkSurfaceKHR &surface
    );
  };
}