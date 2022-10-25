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
      const VkDevice *logical_device,
      const VkRenderPass *render_pass,
      VkPipelineLayout *pipeline_layout,
      VkPipeline *pipeline,
      const std::vector<std::string> vert_shader_files,
      const std::vector<std::string> frag_shader_files
    );
    static void destroyGraphicsPipeline();
  private:
    inline static std::vector<VkShaderModule> m_shader_modules;
    inline static std::vector<VkPipelineShaderStageCreateInfo> m_shader_stages;
    inline static const VkDevice *m_logical_device = nullptr;
    inline static const VkRenderPass *m_render_pass = nullptr;
    inline static VkPipelineLayout *m_pipeline_layout = nullptr;
    inline static VkPipeline *m_pipeline = nullptr;
    
    inline static VkGraphicsPipelineCreateInfo pipeline_info{};

    static std::vector<char> readFile(const std::string &file_name);
    static VkShaderModule createShaderModule
    (
      const VkDevice &logical_device,
      const std::vector<char> &code
    );
  };
}