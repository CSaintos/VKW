// Descriptor.cpp
#include "vkw\Descriptor.hpp"

void vkw::Descriptor::createDescriptorSetLayout
(
  Context &context
)
{
  // link static vars
  m_logical_device = &context.logical_device;
  m_descriptor_set_layout = &context.descriptor_set_layout;

  VkDescriptorSetLayoutBinding ubo_layout_binding{};
  // the binding to the shader descriptor
  ubo_layout_binding.binding = 0;
  // the type of descriptor to create
  ubo_layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  // the amount of descriptors to create
  ubo_layout_binding.descriptorCount = 1;
  // stages of the shader where we access the descriptor
  ubo_layout_binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
  // relevant for image sampling related descriptors
  ubo_layout_binding.pImmutableSamplers = nullptr; // Optional

  VkDescriptorSetLayoutCreateInfo layout_info{};
  layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  layout_info.bindingCount = 1;
  layout_info.pBindings = &ubo_layout_binding;

  if (vkCreateDescriptorSetLayout
    (
      *m_logical_device,
      &layout_info,
      nullptr,
      m_descriptor_set_layout
    ) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create descriptor set layout!");
  }
}

void vkw::Descriptor::destroyDescriptorSetLayout()
{
  vkDestroyDescriptorSetLayout
  (
    *m_logical_device,
    *m_descriptor_set_layout,
    nullptr
  );
}

void vkw::Descriptor::createDescriptorPool
(
  Context &context,
  const int &flight_frame_count
)
{
  // link static vars
  if (m_logical_device == nullptr)
    m_logical_device = &context.logical_device;
  m_descriptor_pool = &context.descriptor_pool;

  /*
  Describes (no pun intended) which descriptor types our
  descriptor sets are going to contain and how many of them.
  */
  VkDescriptorPoolSize pool_size{};
  pool_size.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  pool_size.descriptorCount = 
    static_cast<uint32_t>(flight_frame_count);
  
  VkDescriptorPoolCreateInfo pool_info{};
  pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  pool_info.poolSizeCount = 1;
  pool_info.pPoolSizes = &pool_size;
  // specify max num of descriptor sets that can be alloc
  pool_info.maxSets = static_cast<uint32_t>(flight_frame_count);

  if (vkCreateDescriptorPool
  (
    *m_logical_device,
    &pool_info,
    nullptr,
    m_descriptor_pool
  ) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create descriptor pool");
  }
}

void vkw::Descriptor::createDescriptorSets
(
  Context &context,
  const int &flight_frame_count,
  const VkDeviceSize &buffer_size
)
{
  std::vector<VkDescriptorSetLayout> layouts
    (flight_frame_count, *m_descriptor_set_layout);
  VkDescriptorSetAllocateInfo alloc_info{};
  alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  alloc_info.descriptorPool = *m_descriptor_pool;
  alloc_info.descriptorSetCount = 
    static_cast<uint32_t>(flight_frame_count);
  alloc_info.pSetLayouts = layouts.data();

  context.descriptor_sets.resize(flight_frame_count);
  if (vkAllocateDescriptorSets
  (
    context.logical_device,
    &alloc_info,
    context.descriptor_sets.data()
  ) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to allocate descriptor sets!");
  }

  // Once we allocated the descriptor sets, we now need to populate them.
  for (size_t i = 0; i < flight_frame_count; i++)
  {
    /*
    Descriptors that refer to buffers, like uniform buffers, are configured with
    VkDescriptorBufferInfo struct.
    */
    VkDescriptorBufferInfo buffer_info{};
    buffer_info.buffer = context.uniform_buffers[i];
    buffer_info.offset = 0;
    buffer_info.range = buffer_size;
    /*
    If you're overwriting the whole buffer, in this case, then it's also
    possible to use the VK_WHOLE_SIZE value for the range. The config of
    descriptors is updated using the vkUpdateDescriptorSets function,
    which takes an array of VkWriteDescriptorSet structs as param.
    */
    VkWriteDescriptorSet descriptor_write{};
    descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptor_write.dstSet = context.descriptor_sets[i];
    descriptor_write.dstBinding = 0;
    // It's posisble to update multiple descriptors, starting at
    // index dstArrayElement.
    descriptor_write.dstArrayElement = 0;
    // we specify the descriptor.
    descriptor_write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    // descriptorCount specifies how many array elements you want
    // to update.
    descriptor_write.descriptorCount = 1;
    // bufferInfo for buffer descriptors
    descriptor_write.pBufferInfo = &buffer_info;
    // imageInfo for image descriptors
    descriptor_write.pImageInfo = nullptr; // Optional
    // texelBufferView for buffer view descriptors
    descriptor_write.pTexelBufferView = nullptr; // Optional

    vkUpdateDescriptorSets
    (
      context.logical_device,
      1,
      &descriptor_write,
      0,
      nullptr
    );
  }
}

void vkw::Descriptor::destroyDescriptorPool()
{
  vkDestroyDescriptorPool
  (
    *m_logical_device,
    *m_descriptor_pool,
    nullptr
  );
}