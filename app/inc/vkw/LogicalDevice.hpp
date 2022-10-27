// LogicalDevice.hpp
#pragma once

#include <vulkan\vulkan.hpp>

#include <cstdint>

#include <vector>
#include <set>

#include "vkw\Validation.hpp"
#include "vkw\QueueFamilyIndices.hpp"
#include "vkw\PhysicalDevice.hpp"

namespace vkw 
{
  class LogicalDevice
  {
  public:
    static void createLogicalDevice
    (
      VkPhysicalDevice &physical_device,
      VkSurfaceKHR &surface,
      VkDevice *logical_device,
      VkQueue &graphics_queue,
      VkQueue &present_queue
    );
    static void destroyLogicalDevice();
  private:
    //static inline std::vector<VkDeviceQueueCreateInfo> m_queue_create_infos;
    //static inline VkPhysicalDeviceFeatures m_device_features{};
    //static inline VkDeviceCreateInfo m_create_info{};
    static inline VkDevice *m_logical_device = nullptr;

    LogicalDevice();
  };
}