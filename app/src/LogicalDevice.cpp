// LogicalDevice.cpp
#include "vkw\LogicalDevice.hpp"

void vkw::LogicalDevice::createLogicalDevice
(
  VkPhysicalDevice *physical_device,
  VkDevice *logical_device,
  VkQueue *graphics_queue
)
{
  QueueFamilyIndices indices = 
    PhysicalDevice::findQueueFamilies(*physical_device);
  
  m_queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  m_queue_create_info.queueFamilyIndex = indices.graphics_family.value();
  m_queue_create_info.queueCount = 1;

  float queue_priority = 1.0f;
  m_queue_create_info.pQueuePriorities = &queue_priority;

  m_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  m_create_info.pQueueCreateInfos = &m_queue_create_info;
  m_create_info.queueCreateInfoCount = 1;
  m_create_info.pEnabledFeatures = &m_device_features;
  m_create_info.enabledExtensionCount = 0;

  if (Validation::enable_validation_layers)
  {
    m_create_info.enabledLayerCount = static_cast<uint32_t>(Validation::validation_layers.size());
    m_create_info.ppEnabledLayerNames = Validation::validation_layers.data();
  }
  else
  {
    m_create_info.enabledLayerCount = 0;
  }

  if (vkCreateDevice(*physical_device, &m_create_info, nullptr, logical_device) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create logical device!");
  }

  vkGetDeviceQueue(*logical_device, indices.graphics_family.value(), 0, graphics_queue);

  m_logical_device = logical_device;
}

void vkw::LogicalDevice::destroyLogicalDevice()
{
  vkDestroyDevice(*m_logical_device, nullptr);
}