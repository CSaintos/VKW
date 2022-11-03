// VertexBuffer.cpp
#include "vkw\VertexBuffer.hpp"

void vkw::VertexBuffer::createVertexBuffer
(
  VkDevice *logical_device,
  VkBuffer *vertex_buffer,
  VkPhysicalDevice &physical_device,
  VkDeviceMemory *vertex_buffer_memory,
  std::vector<Vertex> &vertices
)
{
  VkBufferCreateInfo buffer_info{};
  buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  buffer_info.size = 
    (VkDeviceSize) (sizeof(vertices[0]) * vertices.size());
  buffer_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
  buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  if (vkCreateBuffer
    (
      *logical_device,
      &buffer_info,
      nullptr,
      vertex_buffer
    ) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create vertex buffer!");
  }

  m_logical_device = logical_device;
  m_vertex_buffer = vertex_buffer;

  // Once we create the buffer we need to assign memory to it.

  VkMemoryRequirements mem_req{};
  vkGetBufferMemoryRequirements(*m_logical_device, *m_vertex_buffer, &mem_req);

  VkMemoryAllocateInfo alloc_info{};
  alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  alloc_info.allocationSize = mem_req.size;
  alloc_info.memoryTypeIndex = 
    findMemoryType
    (
      mem_req.memoryTypeBits, 
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
      VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
      physical_device
    );
  
  if (vkAllocateMemory
    (
      *m_logical_device,
      &alloc_info,
      nullptr,
      vertex_buffer_memory
    ) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to allocate vertex buffer memory!");
  }

  // Once we allocated memory, we can now assign the buffer to the memory.

  vkBindBufferMemory(*m_logical_device, *vertex_buffer, *vertex_buffer_memory, 0);
  
  m_vertex_buffer_memory = vertex_buffer_memory;

  // Now we map the buffer memory.
  void *data;
  vkMapMemory
  (
    *m_logical_device, 
    *m_vertex_buffer_memory, 
    0, 
    buffer_info.size, 
    0, 
    &data
  );
  memcpy(data, vertices.data(), (size_t) buffer_info.size);
  vkUnmapMemory(*m_logical_device, *m_vertex_buffer_memory);
}

void vkw::VertexBuffer::destroyVertexBuffer()
{
  vkDestroyBuffer(*m_logical_device, *m_vertex_buffer, nullptr);
  vkFreeMemory(*m_logical_device, *m_vertex_buffer_memory, nullptr);
}

uint32_t vkw::VertexBuffer::findMemoryType
(
  const uint32_t &type_filter,
  VkMemoryPropertyFlags properties,
  VkPhysicalDevice &physical_device
)
{
  VkPhysicalDeviceMemoryProperties mem_properties;
  vkGetPhysicalDeviceMemoryProperties(physical_device, &mem_properties);

  for (uint32_t i = 0; i < mem_properties.memoryTypeCount; i++)
  {
    if (type_filter & (1 << i) &&
      (mem_properties.memoryTypes[i].propertyFlags & properties) == properties)
    {
      return i;
    }
  }

  throw std::runtime_error("failed to find suitable memory type!");
}

std::vector<VkVertexInputBindingDescription> vkw::Vertex::getBindingDescription()
{
  std::vector<VkVertexInputBindingDescription> binding_descs(1);

  binding_descs[0].binding = 0;
  binding_descs[0].stride = sizeof(Vertex);
  binding_descs[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

  return binding_descs;
}

std::vector<VkVertexInputAttributeDescription> vkw::Vertex::getAttributeDescriptions()
{
  std::vector<VkVertexInputAttributeDescription> attribute_descs(2);

  attribute_descs[0].binding = 0;
  attribute_descs[0].location = 0;
  attribute_descs[0].format = VK_FORMAT_R32G32_SFLOAT;
  attribute_descs[0].offset = offsetof(Vertex, pos);

  attribute_descs[1].binding = 0;
  attribute_descs[1].location = 1;
  attribute_descs[1].format = VK_FORMAT_R32G32B32_SFLOAT;
  attribute_descs[1].offset = offsetof(Vertex, color);

  return attribute_descs;
}