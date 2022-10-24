// GraphicsPipeline.hpp
#pragma once

#include <vulkan\vulkan.hpp>

#include <cstdint>

#include <string>
#include <vector>
#include <fstream>

namespace vkw
{
  class GraphicsPipeline
  {
  public:
    static void createGraphicsPipeline
    (
      const VkDevice &logical_device,
      const std::vector<std::string> vert_shader_files,
      const std::vector<std::string> frag_shader_files
    );
  private:
    inline static std::vector<VkPipelineShaderStageCreateInfo> m_shader_stages;

    static std::vector<char> readFile(const std::string &file_name);
    static VkShaderModule createShaderModule
    (
      const VkDevice &logical_device,
      const std::vector<char> &code
    );
  };
}