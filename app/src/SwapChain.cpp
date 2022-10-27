// Swapchain.cpp
#include "vkw\Swapchain.hpp"

vkw::SwapchainSupportDetails vkw::Swapchain::querySwapchainSupport
(
  const VkPhysicalDevice &physical_device,
  VkSurfaceKHR &surface
)
{
  SwapchainSupportDetails details;

  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &details.capabilities);

  uint32_t format_count;
  vkGetPhysicalDeviceSurfaceFormatsKHR
  (
    physical_device, 
    surface, 
    &format_count,
    nullptr
  );

  if (format_count != 0)
  {
    details.formats.resize(format_count);
    vkGetPhysicalDeviceSurfaceFormatsKHR
    (
      physical_device,
      surface,
      &format_count,
      details.formats.data()
    );
  }

  uint32_t present_mode_count;
  vkGetPhysicalDeviceSurfacePresentModesKHR
  (
    physical_device, 
    surface, 
    &present_mode_count, 
    nullptr
  );

  if (present_mode_count != 0)
  {
    details.present_modes.resize(present_mode_count);
    vkGetPhysicalDeviceSurfacePresentModesKHR
    (
      physical_device,
      surface,
      &present_mode_count,
      details.present_modes.data()
    );
  }

  return details;
}

VkSurfaceFormatKHR vkw::Swapchain::chooseSwapSurfaceFormat
(
  std::vector<VkSurfaceFormatKHR> &available_formats
)
{
  for (VkSurfaceFormatKHR &available_format : available_formats)
  {
    if (available_format.format == VK_FORMAT_B8G8R8A8_SRGB && 
      available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
    {
      return available_format;
    }
  }

  return available_formats[0];
}

VkPresentModeKHR vkw::Swapchain::chooseSwapPresentMode
(
  std::vector<VkPresentModeKHR> &available_present_modes
)
{
  for (VkPresentModeKHR &available_present_mode : available_present_modes)
  {
    if (available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR)
    {
      return available_present_mode;
    }
  }

  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D vkw::Swapchain::chooseSwapExtent
(
  VkSurfaceCapabilitiesKHR &capabilities,
  const int &width,
  const int &height
)
{
  if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) 
  {
    return capabilities.currentExtent;
  }
  else
  {
    VkExtent2D actual_extent = {
      static_cast<uint32_t>(width),
      static_cast<uint32_t>(height)
    };
    
    actual_extent.width = 
      std::clamp 
      (
        actual_extent.width,
        capabilities.minImageExtent.width,
        capabilities.maxImageExtent.width
      );
    
    actual_extent.height =
      std::clamp
      (
        actual_extent.height,
        capabilities.minImageExtent.height,
        capabilities.maxImageExtent.height
      );
    
    return actual_extent;
  }
}

void vkw::Swapchain::createSwapchain
(
  VkPhysicalDevice &physical_device,
  VkSurfaceKHR &surface,
  const int &width_buffer_size,
  const int &height_buffer_size,
  VkDevice *logical_device,
  VkSwapchainKHR *swapchain,
  std::vector<VkImage> &swapchain_images,
  VkFormat &swapchain_image_format,
  VkExtent2D &swapchain_extent
)
{
  SwapchainSupportDetails swapchain_support = 
    querySwapchainSupport(physical_device, surface);
  
  VkSurfaceFormatKHR surface_format = 
    chooseSwapSurfaceFormat(swapchain_support.formats);
  VkPresentModeKHR present_mode =
    chooseSwapPresentMode(swapchain_support.present_modes);
  VkExtent2D extent = 
    chooseSwapExtent
    (
      swapchain_support.capabilities,
      width_buffer_size,
      height_buffer_size
    );
  
  uint32_t image_count = swapchain_support.capabilities.minImageCount + 1;
  // num 0, is a special value that means that there is no maximum
  if (swapchain_support.capabilities.maxImageCount > 0 &&
    image_count > swapchain_support.capabilities.maxImageCount)
  {
    image_count = swapchain_support.capabilities.maxImageCount;
  }

  VkSwapchainCreateInfoKHR swapchain_info{};
  swapchain_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  swapchain_info.surface = surface;
  swapchain_info.minImageCount = image_count;
  swapchain_info.imageFormat = surface_format.format;
  swapchain_info.imageColorSpace = surface_format.colorSpace;
  swapchain_info.imageExtent = extent;
  swapchain_info.imageArrayLayers = 1;
  swapchain_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  QueueFamilyIndices indices = 
    QueueFamilyIndices::findQueueFamilies(physical_device, surface);
  uint32_t queue_family_indices[] =
    {
      indices.graphics_family.value(),
      indices.present_family.value()
    };
  
  if (indices.graphics_family != indices.present_family)
  {
    swapchain_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    swapchain_info.queueFamilyIndexCount = 2;
    swapchain_info.pQueueFamilyIndices = queue_family_indices;
  }
  else
  {
    swapchain_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchain_info.queueFamilyIndexCount = 0; // Optional
    swapchain_info.pQueueFamilyIndices = nullptr; // Optional
  }

  swapchain_info.preTransform = swapchain_support.capabilities.currentTransform;
  swapchain_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  swapchain_info.presentMode = present_mode;
  // Tell swap chain that we don't care about color of pixels that are obscured
  // ex. when another window is in front of our window.
  // Unless we really care about reading these pixels back, we can just enable
  // clipping for best performance.
  swapchain_info.clipped = VK_TRUE;
  // In Vulkan it's possible that our swap chain becomes invalid/unoptimized when app
  // is running. Ex: when window was resized, the swap chain needs to be recreated
  // and a reference to the old one must be specified. For simplicify, we'll only ever
  // create one swap chain.
  swapchain_info.oldSwapchain = VK_NULL_HANDLE;

  if (vkCreateSwapchainKHR(*logical_device, &swapchain_info, nullptr, swapchain) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create swap chain!");
  }

  m_logical_device = logical_device;
  m_swapchain = swapchain;

  // Retrieve swap chain images
  vkGetSwapchainImagesKHR(*m_logical_device, *m_swapchain, &image_count, nullptr);
  swapchain_images.resize(image_count);
  vkGetSwapchainImagesKHR
    (
      *m_logical_device, 
      *m_swapchain, 
      &image_count, 
      swapchain_images.data()
    );
  // Retrieve swap chain format
  swapchain_image_format = surface_format.format;
  // Retrieve swap chain extent
  swapchain_extent = extent;
}

void vkw::Swapchain::destroySwapchain()
{
  vkDestroySwapchainKHR(*m_logical_device, *m_swapchain, nullptr);
}

void vkw::Swapchain::createImageViews
(
  std::vector<VkImageView> *swapchain_image_views,
  std::vector<VkImage> &swapchain_images,
  VkFormat &swapchain_image_format
)
{
  swapchain_image_views->resize(swapchain_images.size());

  VkImageViewCreateInfo image_view_info{};
  image_view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  image_view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
  image_view_info.format = swapchain_image_format;
  image_view_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
  image_view_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
  image_view_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
  image_view_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
  image_view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  image_view_info.subresourceRange.baseMipLevel = 0;
  image_view_info.subresourceRange.levelCount = 1;
  image_view_info.subresourceRange.baseArrayLayer = 0;
  image_view_info.subresourceRange.layerCount = 1; 

  for (size_t i = 0; i < swapchain_images.size(); i++)
  {
    image_view_info.image = swapchain_images[i];

    if (vkCreateImageView
      (
        *m_logical_device,
        &image_view_info, 
        nullptr,
        &(*swapchain_image_views)[i]
      ) != VK_SUCCESS)
    {
      throw std::runtime_error("failed to create image views!");
    }
  }

  m_swapchain_image_views = swapchain_image_views;
}

void vkw::Swapchain::destroyImageViews()
{
  for (VkImageView image_view : *m_swapchain_image_views)
  {
    vkDestroyImageView(*m_logical_device, image_view, nullptr);
  }
}