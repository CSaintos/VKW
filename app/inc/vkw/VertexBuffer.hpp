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
      std::vector<Vertex> &vertices
    );
    static void destroyVertexBuffer();
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
  private:
    inline static VkDevice *m_logical_device = nullptr;
    inline static VkBuffer *m_vertex_buffer = nullptr;
    inline static VkDeviceMemory *m_vertex_buffer_memory = nullptr;
  };
}