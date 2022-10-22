// SwapChain.cpp
#include "vkw\SwapChain.hpp"

vkw::SwapChainSupportDetails vkw::SwapChain::querySwapChainSupport
(
  const VkPhysicalDevice &physical_device,
  const VkSurfaceKHR &surface
)
{
  SwapChainSupportDetails details;

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

VkSurfaceFormatKHR vkw::SwapChain::chooseSwapSurfaceFormat
(
  const std::vector<VkSurfaceFormatKHR> &available_formats
)
{
  for (const VkSurfaceFormatKHR &available_format : available_formats)
  {
    if (available_format.format == VK_FORMAT_B8G8R8A8_SRGB && 
      available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
    {
      return available_format;
    }
  }

  return available_formats[0];
}

VkPresentModeKHR vkw::SwapChain::chooseSwapPresentMode
(
  const std::vector<VkPresentModeKHR> &available_present_modes
)
{
  for (const VkPresentModeKHR &available_present_mode : available_present_modes)
  {
    if (available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR)
    {
      return available_present_mode;
    }
  }

  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D vkw::SwapChain::chooseSwapExtent
(
  const VkSurfaceCapabilitiesKHR &capabilities,
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

void vkw::SwapChain::createSwapChain
(
  const VkPhysicalDevice &physical_device,
  const VkSurfaceKHR &surface,
  const int &width_buffer_size,
  const int &height_buffer_size,
  VkDevice *logical_device,
  VkSwapchainKHR *swap_chain,
  std::vector<VkImage> &swap_chain_images,
  VkFormat &swap_chain_image_format,
  VkExtent2D &swap_chain_extent
)
{
  SwapChainSupportDetails swap_chain_support = 
    querySwapChainSupport(physical_device, surface);
  
  VkSurfaceFormatKHR surface_format = 
    chooseSwapSurfaceFormat(swap_chain_support.formats);
  VkPresentModeKHR present_mode =
    chooseSwapPresentMode(swap_chain_support.present_modes);
  VkExtent2D extent = 
    chooseSwapExtent
    (
      swap_chain_support.capabilities,
      width_buffer_size,
      height_buffer_size
    );
  
  uint32_t image_count = swap_chain_support.capabilities.minImageCount + 1;
  // num 0, is a special value that means that there is no maximum
  if (swap_chain_support.capabilities.maxImageCount > 0 &&
    image_count > swap_chain_support.capabilities.maxImageCount)
  {
    image_count = swap_chain_support.capabilities.maxImageCount;
  }

  m_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  m_create_info.surface = surface;
  m_create_info.minImageCount = image_count;
  m_create_info.imageFormat = surface_format.format;
  m_create_info.imageColorSpace = surface_format.colorSpace;
  m_create_info.imageExtent = extent;
  m_create_info.imageArrayLayers = 1;
  m_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  QueueFamilyIndices indices = 
    QueueFamilyIndices::findQueueFamilies(physical_device, surface);
  uint32_t queue_family_indices[] =
    {
      indices.graphics_family.value(),
      indices.present_family.value()
    };
  
  if (indices.graphics_family != indices.present_family)
  {
    m_create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    m_create_info.queueFamilyIndexCount = 2;
    m_create_info.pQueueFamilyIndices = queue_family_indices;
  }
  else
  {
    m_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    m_create_info.queueFamilyIndexCount = 0; // Optional
    m_create_info.pQueueFamilyIndices = nullptr; // Optional
  }

  m_create_info.preTransform = swap_chain_support.capabilities.currentTransform;
  m_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  m_create_info.presentMode = present_mode;
  // Tell swap chain that we don't care about color of pixels that are obscured
  // ex. when another window is in front of our window.
  // Unless we really care about reading these pixels back, we can just enable
  // clipping for best performance.
  m_create_info.clipped = VK_TRUE;
  // In Vulkan it's possible that our swap chain becomes invalid/unoptimized when app
  // is running. Ex: when window was resized, the swap chain needs to be recreated
  // and a reference to the old one must be specified. For simplicify, we'll only ever
  // create one swap chain.
  m_create_info.oldSwapchain = VK_NULL_HANDLE;

  if (vkCreateSwapchainKHR(*logical_device, &m_create_info, nullptr, swap_chain) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create swap chain!");
  }

  m_logical_device = logical_device;
  m_swap_chain = swap_chain;

  // Retrieve swap chain images
  vkGetSwapchainImagesKHR(*m_logical_device, *m_swap_chain, &image_count, nullptr);
  swap_chain_images.resize(image_count);
  vkGetSwapchainImagesKHR
    (
      *m_logical_device, 
      *m_swap_chain, 
      &image_count, 
      swap_chain_images.data()
    );
  // Retrieve swap chain format
  swap_chain_image_format = surface_format.format;
  // Retrieve swap chain extent
  swap_chain_extent = extent;
}

void vkw::SwapChain::destroySwapChain()
{
  vkDestroySwapchainKHR(*m_logical_device, *m_swap_chain, nullptr);
}

void vkw::SwapChain::createImageViews
(
  std::vector<VkImageView> *swap_chain_image_views,
  const std::vector<VkImage> &swap_chain_images,
  const VkFormat &swap_chain_image_format
)
{
  swap_chain_image_views->resize(swap_chain_images.size());

  m_image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  m_image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
  m_image_view_create_info.format = swap_chain_image_format;
  m_image_view_create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
  m_image_view_create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
  m_image_view_create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
  m_image_view_create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
  m_image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  m_image_view_create_info.subresourceRange.baseMipLevel = 0;
  m_image_view_create_info.subresourceRange.levelCount = 1;
  m_image_view_create_info.subresourceRange.baseArrayLayer = 0;
  m_image_view_create_info.subresourceRange.layerCount = 1; 

  for (size_t i = 0; i < swap_chain_images.size(); i++)
  {
    m_image_view_create_info.image = swap_chain_images[i];

    if (vkCreateImageView
      (
        *m_logical_device,
        &m_image_view_create_info, 
        nullptr,
        &(*swap_chain_image_views)[i]
      ) != VK_SUCCESS)
    {
      throw std::runtime_error("failed to create image views!");
    }
  }

  m_swap_chain_image_views = swap_chain_image_views;
}

void vkw::SwapChain::destroyImageViews()
{
  for (VkImageView image_view : *m_swap_chain_image_views)
  {
    vkDestroyImageView(*m_logical_device, image_view, nullptr);
  }
}