// PhysicalDevice.cpp
#include "vkw\PhysicalDevice.hpp"

void vkw::PhysicalDevice::pickPhysicalDevice
(
  Context &context
)
{
  uint32_t device_count = 0;
  vkEnumeratePhysicalDevices(context.instance, &device_count, nullptr);

  if (device_count == 0)
  {
    throw std::runtime_error("failed to find GPUs with Vulkan support!");
  }

  std::vector<VkPhysicalDevice> devices(device_count);
  vkEnumeratePhysicalDevices(context.instance, &device_count, devices.data());

  for (const VkPhysicalDevice &device : devices)
  {
    if (isDeviceSuitable(device, context.surface))
    {
      context.physical_device = device;
      break;
    }
  }

  if (context.physical_device == VK_NULL_HANDLE)
  {
    throw std::runtime_error("failed to find a suitable GPU!");
  }
}

bool vkw::PhysicalDevice::isDeviceSuitable
(
  const VkPhysicalDevice &physical_device,
  VkSurfaceKHR &surface
)
{
  QueueFamilyIndices indices = QueueFamilyIndices::findQueueFamilies(physical_device, surface);

  bool extensions_supported = checkDeviceExtensionSupport(physical_device);
  bool swap_chain_adequate = false;
  if (extensions_supported)
  {
    SwapchainSupportDetails swap_chain_support =
      Swapchain::querySwapchainSupport(physical_device, surface);
    swap_chain_adequate = !swap_chain_support.formats.empty() &&
      !swap_chain_support.present_modes.empty();
  }

  return indices.isComplete() && extensions_supported && swap_chain_adequate;
}

bool vkw::PhysicalDevice::checkDeviceExtensionSupport
(
  const VkPhysicalDevice &physical_device
)
{
  uint32_t extension_count;
  vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extension_count, nullptr);

  std::vector<VkExtensionProperties> available_extensions(extension_count);
  vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extension_count, available_extensions.data());

  std::set<std::string> required_extensions(device_extensions.begin(), device_extensions.end());

  for (const VkExtensionProperties &extension : available_extensions)
  {
    required_extensions.erase(extension.extensionName);
  }

  return required_extensions.empty();
}