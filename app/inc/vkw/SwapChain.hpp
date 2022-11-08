// Swapchain.hpp
#pragma once

#include <vulkan\vulkan.hpp>

#include <cstdint>

#include <vector>
#include <limits>
#include <algorithm>

#include "vkw\Context.hpp"
#include "vkw\QueueFamilyIndices.hpp"
#include "vkw\Framebuffer.hpp"

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
      const int &width_buffer_size,
      const int &height_buffer_size,
      Context *context = nullptr
    );
    static void destroySwapchain();
    static void createImageViews();
    static void destroyImageViews();
    static void recreateSwapchain
    (
      std::vector<VkFramebuffer> *swapchain_framebuffers,
      const VkRenderPass &render_pass,
      const int &width_buffer_size,
      const int &height_buffer_size
    );
    static void cleanupSwapchain();
  private:
    Swapchain();

    inline static Context *m_context = nullptr;
    inline static VkDevice *m_logical_device = nullptr;
    inline static VkSwapchainKHR *m_swapchain = nullptr;
    inline static std::vector<VkImageView> *m_swapchain_image_views = nullptr;
    inline static VkPhysicalDevice *m_physical_device = nullptr;
    inline static VkSurfaceKHR *m_surface = nullptr;
    inline static std::vector<VkImage> *m_swapchain_images = nullptr;
    inline static VkFormat *m_swapchain_image_format = nullptr;
    inline static VkExtent2D *m_swapchain_extent = nullptr;
    inline static std::vector<VkFramebuffer> *m_swapchain_framebuffers = nullptr;

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