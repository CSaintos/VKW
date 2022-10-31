// Vertex.cpp
#include "Vertex.hpp"

VkVertexInputBindingDescription Vertex::getBindingDescription()
{
  VkVertexInputBindingDescription binding_desc{};

  binding_desc.binding = 0;
  binding_desc.stride = sizeof(Vertex);
  binding_desc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

  return binding_desc;
}

std::array<VkVertexInputAttributeDescription, 2> Vertex::getAttributeDescriptions()
{
  std::array<VkVertexInputAttributeDescription, 2> attribute_descs{};

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