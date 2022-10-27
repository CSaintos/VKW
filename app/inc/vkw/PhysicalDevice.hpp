// PhysicalDevice.hpp
#pragma once

#include <vulkan\vulkan.hpp>

#include <cstdint>

#include <set>
#include <string>
#include <vector>

#include "vkw\QueueFamilyIndices.hpp"
#include "vkw\Swapchain.hpp"

namespace vkw
{
  class PhysicalDevice
  {
  public:
    static inline const std::vector<const char*> device_extensions = {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    static void pickPhysicalDevice
    (
      VkInstance &vk_instance, 
      VkPhysicalDevice &physical_device,
      VkSurfaceKHR &surface
    );
  private:
    static bool isDeviceSuitable
    (
      const VkPhysicalDevice &device,
      VkSurfaceKHR &surface
    );
    static bool checkDeviceExtensionSupport(const VkPhysicalDevice &physical_device);
  };
}