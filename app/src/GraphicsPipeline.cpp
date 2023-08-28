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
  VkDevice &logical_device,
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
  Context &context,
  const std::vector<std::string> &vert_shader_files,
  const std::vector<std::string> &frag_shader_files,
  const std::vector<VkVertexInputBindingDescription> &binding_descs,
  const std::vector<VkVertexInputAttributeDescription> &attrib_descs
)
{
  // link static vars
  m_logical_device = &context.logical_device;
  m_render_pass = &context.render_pass;
  m_pipeline_layout = &context.pipeline_layout;
  m_pipeline = &context.graphics_pipeline;

  for (std::string vert_shader_file : vert_shader_files)
  {
    VkShaderModule vert_shader_module =
      createShaderModule
      (
        *m_logical_device,
        readFile(vert_shader_file)
      );

    VkPipelineShaderStageCreateInfo vert_shader_stage_info{};
    vert_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vert_shader_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vert_shader_stage_info.module = vert_shader_module;
    vert_shader_stage_info.pName = "main";

    m_shader_stages.push_back(vert_shader_stage_info);
    m_shader_modules.push_back(vert_shader_module);
  }

  for (std::string frag_shader_file : frag_shader_files)
  {
    VkShaderModule frag_shader_module =
      createShaderModule
      (
        *m_logical_device,
        readFile(frag_shader_file)
      );

    VkPipelineShaderStageCreateInfo frag_shader_stage_info{};
    frag_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    frag_shader_stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    frag_shader_stage_info.module = frag_shader_module;
    frag_shader_stage_info.pName = "main";

    m_shader_stages.push_back(frag_shader_stage_info);
    m_shader_modules.push_back(frag_shader_module);
  }
  
  // Dynamic State fixed function
  // used to keep out the viewport and line and blend constants
  // whenever the pipeline is recreated, so therefore they need to be
  // specified during drawing time.
  std::vector<VkDynamicState> dynamic_states = 
  {
    VK_DYNAMIC_STATE_VIEWPORT,
    VK_DYNAMIC_STATE_SCISSOR
  };

  VkPipelineDynamicStateCreateInfo dynamic_state_info{};
  dynamic_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  dynamic_state_info.dynamicStateCount = static_cast<uint32_t>(dynamic_states.size());
  dynamic_state_info.pDynamicStates = dynamic_states.data();

  // Vertex Input State Fixed Function
  VkPipelineVertexInputStateCreateInfo vertex_input_info{};
  vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertex_input_info.vertexBindingDescriptionCount = static_cast<uint32_t>(binding_descs.size());
  vertex_input_info.pVertexBindingDescriptions = binding_descs.data();
  vertex_input_info.vertexAttributeDescriptionCount = static_cast<uint32_t>(attrib_descs.size());
  vertex_input_info.pVertexAttributeDescriptions = attrib_descs.data();

  // Input Assembly describes what kind of geometry will be drawn
  // from the vertices and if primitive restart should be enabled.
  VkPipelineInputAssemblyStateCreateInfo input_assembly{};
  input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  input_assembly.primitiveRestartEnable = VK_FALSE;

  VkPipelineViewportStateCreateInfo viewport_state_info{};
  viewport_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewport_state_info.viewportCount = 1;
  viewport_state_info.scissorCount = 1;

  VkPipelineRasterizationStateCreateInfo rasterization_info{};
  rasterization_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterization_info.depthClampEnable = VK_FALSE;
  rasterization_info.rasterizerDiscardEnable = VK_FALSE; // if enabled, disables any output to frame buffer
  rasterization_info.polygonMode = VK_POLYGON_MODE_FILL; // fill with fragments
  rasterization_info.lineWidth = 1.0f; // describes thickness of lines, in terms of # of frags. Any line thicker than 1.0f requires to enable wideLines GPU features.
  rasterization_info.cullMode = VK_CULL_MODE_BACK_BIT;
  rasterization_info.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE; // due to glm
  rasterization_info.depthBiasEnable = VK_FALSE;
  rasterization_info.depthBiasConstantFactor = 0.0f; // Optional
  rasterization_info.depthBiasClamp = 0.0f; // Optional
  rasterization_info.depthBiasSlopeFactor = 0.0f; // Optional

  // Configuring multi-sampling
  //  one of the ways to perform anti-aliasing.
  //  works by combinging frag shader results of mutiple polygons that rasterize to 
  //  the same pixel. Aliasing mostly occurs along edges. It doesn't need to run the frag
  //  shader mutliple times if only one polygon maps to a pixel.
  VkPipelineMultisampleStateCreateInfo multisampling_info{};
  multisampling_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisampling_info.sampleShadingEnable = VK_FALSE;
  multisampling_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  multisampling_info.minSampleShading = 1.0f; // Optional
  multisampling_info.pSampleMask = nullptr; // Optional
  multisampling_info.alphaToCoverageEnable = VK_FALSE; // Optional
  multisampling_info.alphaToOneEnable = VK_FALSE; // Optional

  // Section for Depth and Stencil testing 
  //

  // Color blending
  // Two ways to achieve this:
  // - Mix the old and new value to produce a final color
  // - Combine the old and new value using a bitwise operation
  // Two structs to configure color blending:
  // - VkPipelineColorBlendAttachmentState : contains config per attached framebuffer
  // - VkPipelineColorBlendStateCreateInfo : contains the global color blending settings.
  VkPipelineColorBlendAttachmentState color_blend_attachment{};
  color_blend_attachment.colorWriteMask = 
    VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
    VK_COLOR_COMPONENT_A_BIT;
  color_blend_attachment.blendEnable = VK_FALSE;
  color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
  color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
  color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
  color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
  color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
  color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

  VkPipelineColorBlendStateCreateInfo color_blend_info{};
  color_blend_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  color_blend_info.logicOpEnable = VK_FALSE;
  color_blend_info.logicOp = VK_LOGIC_OP_COPY; // Optional
  color_blend_info.attachmentCount = 1;
  color_blend_info.pAttachments = &color_blend_attachment;
  color_blend_info.blendConstants[0] = 0.0f; // Optional
  color_blend_info.blendConstants[1] = 0.0f; // Optional
  color_blend_info.blendConstants[2] = 0.0f; // Optional
  color_blend_info.blendConstants[3] = 0.0f; // Optional

  VkPipelineLayoutCreateInfo pipeline_layout_info{};
  pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  
  if (context.descriptor_set_layout)
  {
    pipeline_layout_info.setLayoutCount = 1;
    pipeline_layout_info.pSetLayouts = &context.descriptor_set_layout;
  }
  else
  {
    pipeline_layout_info.setLayoutCount = 0;
    pipeline_layout_info.pSetLayouts = nullptr; // Optional
  }

  pipeline_layout_info.pushConstantRangeCount = 0; // Optional
  pipeline_layout_info.pPushConstantRanges = nullptr; // Optional

  if (vkCreatePipelineLayout
    (
      *m_logical_device, 
      &pipeline_layout_info, 
      nullptr, 
      m_pipeline_layout
    ) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create pipeline layout!");
  }

  VkGraphicsPipelineCreateInfo pipeline_info{};
  pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipeline_info.stageCount = m_shader_stages.size();
  pipeline_info.pStages = m_shader_stages.data();
  pipeline_info.pVertexInputState = &vertex_input_info;
  pipeline_info.pInputAssemblyState = &input_assembly;
  pipeline_info.pViewportState = &viewport_state_info;
  pipeline_info.pRasterizationState = &rasterization_info;
  pipeline_info.pMultisampleState = &multisampling_info;
  pipeline_info.pDepthStencilState = nullptr; // Optional
  pipeline_info.pColorBlendState = &color_blend_info;
  pipeline_info.pDynamicState = &dynamic_state_info;
  pipeline_info.layout = *m_pipeline_layout;
  pipeline_info.renderPass = *m_render_pass;
  pipeline_info.subpass = 0; // index of sub pass where graphics pipeline is used.
  // Vulkan allows you to create a new graphics pipeline by deriving
  // from an existing pipeline.
  pipeline_info.basePipelineHandle = VK_NULL_HANDLE; // Optional
  pipeline_info.basePipelineIndex = -1; // Optional

  if (vkCreateGraphicsPipelines
    (
      *m_logical_device,
      VK_NULL_HANDLE,
      1,
      &pipeline_info,
      nullptr,
      m_pipeline
    ) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create graphics pipeline!");
  }

  for (VkShaderModule shader_module : m_shader_modules)
  {
    vkDestroyShaderModule(*m_logical_device, shader_module, nullptr);
  }
}

void vkw::GraphicsPipeline::destroyGraphicsPipeline()
{
  vkDestroyPipeline(*m_logical_device, *m_pipeline, nullptr);
  vkDestroyPipelineLayout(*m_logical_device, *m_pipeline_layout, nullptr);
}