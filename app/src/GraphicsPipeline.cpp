// GraphicsPipeline.cpp
#include "vkw\GraphicsPipeline.hpp"

std::vector<char> vkw::GraphicsPipeline::readFile(const std::string &file_name)
{
  std::ifstream file(file_name, std::ios::ate | std::ios::binary);

  if (!file.is_open())
  {
    throw std::runtime_error("failed to open file!");
  }

  size_t file_size = (size_t) file.tellg();
  std::vector<char> buffer(file_size);

  file.seekg(0);
  file.read(buffer.data(), file_size);

  file.close();

  return buffer;
}

VkShaderModule vkw::GraphicsPipeline::createShaderModule
(
  const VkDevice &logical_device,
  const std::vector<char> &code
)
{
  VkShaderModuleCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  create_info.codeSize = code.size();
  create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());

  VkShaderModule shader_module;
  if (vkCreateShaderModule
    (
      logical_device, 
      &create_info, 
      nullptr, 
      &shader_module
    ) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create shader module!");
  }

  return shader_module;
}

void vkw::GraphicsPipeline::createGraphicsPipeline
(
  const VkDevice &logical_device,
  const std::vector<std::string> vert_shader_files,
  const std::vector<std::string> frag_shader_files
)
{
  for (std::string vert_shader_file : vert_shader_files)
  {
    VkShaderModule vert_shader_module =
      createShaderModule
      (
        logical_device,
        readFile(vert_shader_file)
      );

    VkPipelineShaderStageCreateInfo vert_shader_stage_info{};
    vert_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vert_shader_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vert_shader_stage_info.module = vert_shader_module;
    vert_shader_stage_info.pName = "main";

    m_shader_stages.push_back(vert_shader_stage_info);

    vkDestroyShaderModule(logical_device, vert_shader_module, nullptr);
  }

  for (std::string frag_shader_file : frag_shader_files)
  {
    VkShaderModule frag_shader_module =
      createShaderModule
      (
        logical_device,
        readFile(frag_shader_file)
      );

    VkPipelineShaderStageCreateInfo frag_shader_stage_info{};
    frag_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    frag_shader_stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    frag_shader_stage_info.module = frag_shader_module;
    frag_shader_stage_info.pName = "main";

    m_shader_stages.push_back(frag_shader_stage_info);

    vkDestroyShaderModule(logical_device, frag_shader_module, nullptr);
  }
}