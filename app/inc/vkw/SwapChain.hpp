// SwapChain.hpp
#pragma once

#include <vulkan\vulkan.hpp>

#include <cstdint>

#include <vector>
#include <limits>
#include <algorithm>

#include "vkw\QueueFamilyIndices.hpp"

namespace vkw
{
  struct SwapChainSupportDetails
  {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> present_modes;
  };
  class SwapChain
  {
  public:
    static SwapChainSupportDetails querySwapChainSupport
    (
      const VkPhysicalDevice &physical_device,
      const VkSurfaceKHR &surface
    );
    static void createSwapChain
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
    );
    static void destroySwapChain();
    static void createImageViews
    (
      std::vector<VkImageView> *swap_chain_image_views,
      const std::vector<VkImage> &swap_chain_images,
      const VkFormat &swap_chain_image_format
    );
    static void destroyImageViews();
  private:
    SwapChain();

    inline static VkSwapchainCreateInfoKHR m_create_info{};
    inline static VkImageViewCreateInfo m_image_view_create_info{};

    inline static VkDevice *m_logical_device = nullptr;
    inline static VkSwapchainKHR *m_swap_chain = nullptr;
    inline static std::vector<VkImageView> *m_swap_chain_image_views = nullptr;

    static VkSurfaceFormatKHR chooseSwapSurfaceFormat
    (
      const std::vector<VkSurfaceFormatKHR> &available_formats
    );
    static VkPresentModeKHR chooseSwapPresentMode
    (
      const std::vector<VkPresentModeKHR> &available_present_modes
    );
    static VkExtent2D chooseSwapExtent
    (
      const VkSurfaceCapabilitiesKHR& capabilities,
      const int &width,
      const int &height
    );
  };
}