// LogicalDevice.cpp
#include "vkw\LogicalDevice.hpp"

void vkw::LogicalDevice::createLogicalDevice
(
  Context &context
  //VkPhysicalDevice &physical_device,
  //VkSurfaceKHR &surface,
  //VkDevice *logical_device,
  //VkQueue &graphics_queue,
  //VkQueue &present_queue
)
{
  // link static vars
  m_logical_device = &context.logical_device;

  QueueFamilyIndices indices = 
    QueueFamilyIndices::findQueueFamilies(context.physical_device, context.surface);
  
  std::set<uint32_t> unique_queue_families = 
  {
    indices.graphics_family.value(),
    indices.present_family.value()
  };

  std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
  float queue_priority = 1.0f;
  for (uint32_t queue_family : unique_queue_families)
  {
    VkDeviceQueueCreateInfo queue_create_info{};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = queue_family;
    queue_create_info.queueCount = 1;
    queue_create_info.pQueuePriorities = &queue_priority;
    queue_create_infos.push_back(queue_create_info);
  }

  VkPhysicalDeviceFeatures device_features{};

  VkDeviceCreateInfo logical_device_info{};
  logical_device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  logical_device_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
  logical_device_info.pQueueCreateInfos = queue_create_infos.data();
  logical_device_info.pEnabledFeatures = &device_features;
  logical_device_info.enabledExtensionCount = 
    static_cast<uint32_t>(PhysicalDevice::device_extensions.size());
  logical_device_info.ppEnabledExtensionNames = PhysicalDevice::device_extensions.data();

  if (Validation::enable_validation_layers)
  {
    logical_device_info.enabledLayerCount = static_cast<uint32_t>(Validation::validation_layers.size());
    logical_device_info.ppEnabledLayerNames = Validation::validation_layers.data();
  }
  else
  {
    logical_device_info.enabledLayerCount = 0;
  }

  if (vkCreateDevice
  (
    context.physical_device, 
    &logical_device_info, 
    nullptr, 
    m_logical_device
  ) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create logical device!");
  }

  vkGetDeviceQueue
  (
    *m_logical_device, 
    indices.graphics_family.value(), 
    0, 
    &context.graphics_queue
  );
  vkGetDeviceQueue
  (
    *m_logical_device, 
    indices.present_family.value(), 
    0, 
    &context.present_queue
  );
}

void vkw::LogicalDevice::destroyLogicalDevice()
{
  vkDestroyDevice(*m_logical_device, nullptr);
}