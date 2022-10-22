// LogicalDevice.cpp
#include "vkw\LogicalDevice.hpp"

void vkw::LogicalDevice::createLogicalDevice
(
  const VkPhysicalDevice &physical_device,
  const VkSurfaceKHR &surface,
  VkDevice *logical_device,
  VkQueue &graphics_queue,
  VkQueue &present_queue
)
{
  QueueFamilyIndices indices = 
    QueueFamilyIndices::findQueueFamilies(physical_device, surface);
  
  std::set<uint32_t> unique_queue_families = 
  {
    indices.graphics_family.value(),
    indices.present_family.value()
  };

  float queue_priority = 1.0f;
  for (uint32_t queue_family : unique_queue_families)
  {
    VkDeviceQueueCreateInfo queue_create_info{};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = queue_family;
    queue_create_info.queueCount = 1;
    queue_create_info.pQueuePriorities = &queue_priority;
    m_queue_create_infos.push_back(queue_create_info);
  }

  m_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  m_create_info.queueCreateInfoCount = static_cast<uint32_t>(m_queue_create_infos.size());
  m_create_info.pQueueCreateInfos = m_queue_create_infos.data();
  m_create_info.pEnabledFeatures = &m_device_features;
  m_create_info.enabledExtensionCount = 
    static_cast<uint32_t>(PhysicalDevice::device_extensions.size());
  m_create_info.ppEnabledExtensionNames = PhysicalDevice::device_extensions.data();

  if (Validation::enable_validation_layers)
  {
    m_create_info.enabledLayerCount = static_cast<uint32_t>(Validation::validation_layers.size());
    m_create_info.ppEnabledLayerNames = Validation::validation_layers.data();
  }
  else
  {
    m_create_info.enabledLayerCount = 0;
  }

  if (vkCreateDevice(physical_device, &m_create_info, nullptr, logical_device) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create logical device!");
  }

  vkGetDeviceQueue(*logical_device, indices.graphics_family.value(), 0, &graphics_queue);
  vkGetDeviceQueue(*logical_device, indices.present_family.value(), 0, &present_queue);

  m_logical_device = logical_device;
}

void vkw::LogicalDevice::destroyLogicalDevice()
{
  vkDestroyDevice(*m_logical_device, nullptr);
}