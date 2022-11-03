// VertexBuffer.hpp
#pragma once

#include <vulkan\vulkan.hpp>

#include <cstdint>

#include <array>
#include <vector>

namespace vkw
{
  struct Vertex
  {
    std::array<float, 2> pos;
    std::array<float, 3> color;

    static std::vector<VkVertexInputBindingDescription> getBindingDescription();
    static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
  };
  
  class VertexBuffer
  {
  public:
    static void createVertexBuffer
    (
      VkDevice *logical_device,
      VkBuffer *vertex_buffer,
      VkPhysicalDevice &physical_device,
      VkDeviceMemory *vertex_buffer_memory,
      std::vector<Vertex> &vertices,
      VkCommandPool &command_pool,
      VkQueue &graphics_queue
    );
    static void destroyVertexBuffer();
  private:
    inline static VkDevice *m_logical_device = nullptr;
    inline static VkBuffer *m_vertex_buffer = nullptr;
    inline static VkDeviceMemory *m_vertex_buffer_memory = nullptr;
  };

  class Buffer
  {
  public:
    static void createBuffer
    (
      const VkDeviceSize &size,
      VkBufferUsageFlags usage,
      VkMemoryPropertyFlags properties,
      VkBuffer &buffer,
      VkDeviceMemory &buffer_memory,
      VkDevice &logical_device,
      VkPhysicalDevice &physical_device
    );
    /**
     * Graphics cards offer different types of memory to allocate.
     * Each type varies in operations and performance.
     * We combine the requirements of the buffer and our app requirements
     * to find the right type.
     * 
     * @param type_filter used to specify bit field of memory types suitable.
     * @param properties our needed properties of the app
     * @param physical_device used to get memory properties compatibility
     * 
     * @return index of favorable memory type
    */
    static uint32_t findMemoryType
    (
      const uint32_t &type_filter,
      VkMemoryPropertyFlags properties,
      VkPhysicalDevice &physical_device 
    );
    static void copyBuffer
    (
      VkBuffer &src_buffer,
      VkBuffer &dst_buffer,
      const VkDeviceSize &size,
      VkDevice &logical_device,
      VkCommandPool &command_pool,
      VkQueue &graphics_queue
    );
  };
}