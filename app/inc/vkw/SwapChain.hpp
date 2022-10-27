// Swapchain.hpp
#pragma once

#include <vulkan\vulkan.hpp>

#include <cstdint>

#include <vector>
#include <limits>
#include <algorithm>

#include "vkw\QueueFamilyIndices.hpp"

namespace vkw
{
  struct SwapchainSupportDetails
  {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> present_modes;
  };
  class Swapchain
  {
  public:
    static SwapchainSupportDetails querySwapchainSupport
    (
      const VkPhysicalDevice &physical_device,
      VkSurfaceKHR &surface
    );
    static void createSwapchain
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
    );
    static void destroySwapchain();
    static void createImageViews
    (
      std::vector<VkImageView> *swapchain_image_views,
      std::vector<VkImage> &swapchain_images,
      VkFormat &swapchain_image_format
    );
    static void destroyImageViews();
  private:
    Swapchain();

    inline static VkDevice *m_logical_device = nullptr;
    inline static VkSwapchainKHR *m_swapchain = nullptr;
    inline static std::vector<VkImageView> *m_swapchain_image_views = nullptr;

    static VkSurfaceFormatKHR chooseSwapSurfaceFormat
    (
      std::vector<VkSurfaceFormatKHR> &available_formats
    );
    static VkPresentModeKHR chooseSwapPresentMode
    (
      std::vector<VkPresentModeKHR> &available_present_modes
    );
    static VkExtent2D chooseSwapExtent
    (
      VkSurfaceCapabilitiesKHR& capabilities,
      const int &width,
      const int &height
    );
  };
}