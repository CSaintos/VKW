// QueueFamilyIndices.cpp
#include "vkw\QueueFamilyIndices.hpp"

vkw::QueueFamilyIndices vkw::QueueFamilyIndices::findQueueFamilies
(
  const VkPhysicalDevice &physical_device,
  VkSurfaceKHR &surface
)
{
  QueueFamilyIndices indices;

  uint32_t queue_family_count = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, nullptr);

  std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
  vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_families.data());

  int i = 0;
  for (const VkQueueFamilyProperties &queue_family : queue_families)
  {
    if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
    {
      indices.graphics_family = i;
    }

    VkBool32 present_support = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, surface, &present_support);

    if (present_support)
    {
      indices.present_family = i;
    }

    if (indices.isComplete())
    {
      break;
    }

    i++;
  }

  return indices;
}