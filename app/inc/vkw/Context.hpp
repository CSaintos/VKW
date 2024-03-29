// Context.hpp
#pragma once

#include <vulkan\vulkan.hpp>

#include <cstdint>

#include <vector>
#include <optional>

namespace vkw
{
  struct Context
  {
    VkInstance instance;
    VkDebugUtilsMessengerEXT debug_messenger;
    VkPhysicalDevice physical_device = VK_NULL_HANDLE;
    VkDevice logical_device;
    VkQueue graphics_queue;
    VkQueue present_queue;
    VkSurfaceKHR surface;
    VkSwapchainKHR swapchain;
    std::vector<VkImage> swapchain_images;
    VkFormat swapchain_image_format;
    VkExtent2D swapchain_extent;
    std::vector<VkImageView> swapchain_image_views;
    VkRenderPass render_pass;
    //std::optional<VkDescriptorSetLayout> descriptor_set_layout;
    VkDescriptorSetLayout descriptor_set_layout;
    VkPipelineLayout pipeline_layout;
    VkPipeline graphics_pipeline;
    std::vector<VkFramebuffer> swapchain_framebuffers;
    VkCommandPool command_pool;
    std::vector<VkCommandBuffer> command_buffers;
    std::vector<VkSemaphore> image_available_semaphores;
    std::vector<VkSemaphore> render_finished_semaphores;
    std::vector<VkFence> in_flight_fences;
    uint32_t current_frame = 0;
    bool framebuffer_resized = false;
    VkBuffer vertex_buffer;
    VkDeviceMemory vertex_buffer_memory;
    VkBuffer index_buffer;
    VkDeviceMemory index_buffer_memory;
    std::vector<VkBuffer> uniform_buffers;
    std::vector<VkDeviceMemory> uniform_buffers_memory;
    std::vector<void *> uniform_buffers_mapped;
    VkDescriptorPool descriptor_pool;
    std::vector<VkDescriptorSet> descriptor_sets;
  };
}
