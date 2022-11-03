// VertexBuffer.cpp
#include "vkw\VertexBuffer.hpp"

void vkw::VertexBuffer::createVertexBuffer
(
  VkDevice *logical_device,
  VkBuffer *vertex_buffer,
  VkPhysicalDevice &physical_device,
  VkDeviceMemory *vertex_buffer_memory,
  std::vector<Vertex> &vertices,
  VkCommandPool &command_pool,
  VkQueue &graphics_queue
)
{
  // link members
  m_logical_device = logical_device;

  VkDeviceSize buffer_size = 
    sizeof(vertices[0]) * vertices.size();

  VkBuffer staging_buffer;
  VkDeviceMemory staging_buffer_memory;
  Buffer::createBuffer
  (
    buffer_size,
    VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
    staging_buffer,
    staging_buffer_memory,
    *m_logical_device,
    physical_device
  );

  void *data;
  vkMapMemory
  (
    *m_logical_device,
    staging_buffer_memory,
    0,
    buffer_size,
    0,
    &data
  );
  memcpy(data, vertices.data(), (size_t) buffer_size);
  vkUnmapMemory(*m_logical_device, staging_buffer_memory);

  // link members
  m_vertex_buffer = vertex_buffer;
  m_vertex_buffer_memory = vertex_buffer_memory;

  /*
  We're now going to use the staging buffer and its memory
  to map and copy the vertex data.

  Using two flags:
  - VK_BUFFER_USAGE_TRANSFER_SRC_BIT: buffer can be used as
  source in a memory transfer operation.
  - VK_BUFFER_USAGE_TRANSFER_DST_BIT: buffer can be used as
  destination in a memory transfer operation.
  */

  Buffer::createBuffer
  (
    buffer_size,
    VK_BUFFER_USAGE_TRANSFER_DST_BIT |
    VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
    *m_vertex_buffer,
    *m_vertex_buffer_memory,
    *m_logical_device,
    physical_device
  );

  Buffer::copyBuffer
  (
    staging_buffer,
    *vertex_buffer,
    buffer_size,
    *m_logical_device,
    command_pool,
    graphics_queue
  );

  vkDestroyBuffer(*m_logical_device, staging_buffer, nullptr);
  vkFreeMemory(*m_logical_device, staging_buffer_memory, nullptr);
}

void vkw::VertexBuffer::destroyVertexBuffer()
{
  vkDestroyBuffer(*m_logical_device, *m_vertex_buffer, nullptr);
  vkFreeMemory(*m_logical_device, *m_vertex_buffer_memory, nullptr);
}

uint32_t vkw::Buffer::findMemoryType
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

void vkw::Buffer::createBuffer
(
  const VkDeviceSize &size,
  VkBufferUsageFlags usage,
  VkMemoryPropertyFlags properties,
  VkBuffer &buffer,
  VkDeviceMemory &buffer_memory,
  VkDevice &logical_device,
  VkPhysicalDevice &physical_device
)
{
  VkBufferCreateInfo buffer_info{};
  buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  buffer_info.size = size;
  buffer_info.usage = usage;
  buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  if (vkCreateBuffer
    (
      logical_device,
      &buffer_info,
      nullptr,
      &buffer
    ) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create buffer!");
  }

  VkMemoryRequirements mem_req;
  vkGetBufferMemoryRequirements(logical_device, buffer, &mem_req);

  VkMemoryAllocateInfo alloc_info{};
  alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  alloc_info.allocationSize = mem_req.size;
  alloc_info.memoryTypeIndex = 
    findMemoryType(mem_req.memoryTypeBits, properties, physical_device);

  if (vkAllocateMemory
    (
      logical_device,
      &alloc_info,
      nullptr,
      &buffer_memory
    ) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to allocate buffer memory!");
  }

  vkBindBufferMemory
  (
    logical_device,
    buffer,
    buffer_memory,
    0
  );
}

void vkw::Buffer::copyBuffer
(
  VkBuffer &src_buffer,
  VkBuffer &dst_buffer,
  const VkDeviceSize &size,
  VkDevice &logical_device,
  VkCommandPool &command_pool,
  VkQueue &graphics_queue
)
{
  VkCommandBufferAllocateInfo alloc_info{};
  alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  alloc_info.commandPool = command_pool;
  alloc_info.commandBufferCount = 1;

  VkCommandBuffer command_buffer;
  vkAllocateCommandBuffers
  (
    logical_device, 
    &alloc_info, 
    &command_buffer
  );

  VkCommandBufferBeginInfo begin_info{};
  begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

  vkBeginCommandBuffer(command_buffer, &begin_info);

    VkBufferCopy copy_region{};
    copy_region.srcOffset = 0; // Optional
    copy_region.dstOffset = 0; // Optional
    copy_region.size = size;
    vkCmdCopyBuffer
    (
      command_buffer,
      src_buffer,
      dst_buffer,
      1,
      &copy_region
    );
  
  vkEndCommandBuffer(command_buffer);

  VkSubmitInfo submit_info{};
  submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers = &command_buffer;

  vkQueueSubmit(graphics_queue, 1, &submit_info, VK_NULL_HANDLE);
  vkQueueWaitIdle(graphics_queue);

  vkFreeCommandBuffers
  (
    logical_device,
    command_pool,
    1,
    &command_buffer
  );
}