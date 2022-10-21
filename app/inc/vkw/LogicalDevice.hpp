// LogicalDevice.hpp
#pragma once

#include <vulkan\vulkan.hpp>

#include <cstdint>

#include "vkw\Validation.hpp"
#include "vkw\PhysicalDevice.hpp"

namespace vkw 
{
  class LogicalDevice
  {
  public:
    static void createLogicalDevice
    (
      VkPhysicalDevice *physical_device,
      VkDevice *logical_device,
      VkQueue *graphics_queue
    );
    static void destroyLogicalDevice();
  private:
    static inline VkDeviceQueueCreateInfo m_queue_create_info{};
    static inline VkPhysicalDeviceFeatures m_device_features{};
    static inline VkDeviceCreateInfo m_create_info{};
    static inline VkDevice *m_logical_device = nullptr;
  };
}