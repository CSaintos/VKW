// PhysicalDevice.cpp
#include "vkw\PhysicalDevice.hpp"

void vkw::PhysicalDevice::pickPhysicalDevice
(
  const VkInstance &vk_instance,
  VkPhysicalDevice &physical_device,
  const VkSurfaceKHR &surface
)
{
  uint32_t device_count = 0;
  vkEnumeratePhysicalDevices(vk_instance, &device_count, nullptr);

  if (device_count == 0)
  {
    throw std::runtime_error("failed to find GPUs with Vulkan support!");
  }

  std::vector<VkPhysicalDevice> devices(device_count);
  vkEnumeratePhysicalDevices(vk_instance, &device_count, devices.data());

  for (const VkPhysicalDevice &device : devices)
  {
    if (isDeviceSuitable(device, surface))
    {
      physical_device = device;
      break;
    }
  }

  if (physical_device == VK_NULL_HANDLE)
  {
    throw std::runtime_error("failed to find a suitable GPU!");
  }
}

bool vkw::PhysicalDevice::isDeviceSuitable
(
  const VkPhysicalDevice &device,
  const VkSurfaceKHR &surface
)
{
  QueueFamilyIndices indices = findQueueFamilies(device, surface);

  return indices.isComplete();
}

vkw::QueueFamilyIndices vkw::PhysicalDevice::findQueueFamilies
(
  const VkPhysicalDevice &device,
  const VkSurfaceKHR &surface
)
{
  QueueFamilyIndices indices;

  uint32_t queue_family_count = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);

  std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());

  int i = 0;
  for (const VkQueueFamilyProperties &queue_family : queue_families)
  {
    if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
    {
      indices.graphics_family = i;
    }

    VkBool32 present_support = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &present_support);

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