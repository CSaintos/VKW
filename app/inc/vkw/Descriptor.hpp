// Descriptor.hpp
#pragma once

#include <vulkan\vulkan.hpp>

#include <vector>
#include <array>
#include <optional>

#include "vkw\Context.hpp"

namespace vkw 
{
  /**
   * A descriptor is a way for shaders to freely access
   * resources like buffers and images.
   * Usage of descriptors consists of three parts:
   *  1. Specify a descriptor layout during pipeline creation.
   *    - the layout specifies the types of resources that are
   *      going to be accessed by the pipeline, just like a
   *      render pass specifies the types of attachments that
   *      will be accessed.
   *  2. Allocate a descriptor set from a descriptor pool.
   *    - the descriptor set specifies the actual buffer or 
   *      image res that will be bound to the descriptors,
   *      like a framebuffer specifies the actual image views
   *      to bind to render pass attachments.
   *    - The set is then bound for the drawing commands like
   *      the vertex buffers and framebuffer.
   *  3. Bind the descriptor set during rendering.
   * Types of descriptors:
   * - UBO (uniform buffer object)
  */
  class Descriptor
  {
  public:
    static void createDescriptorSetLayout
    (
      Context &context
    );
    static void destroyDescriptorSetLayout();
    static void createDescriptorPool
    (
      Context &context,
      const int &flight_frame_count
    );
    static void createDescriptorSets
    (
      Context &context,
      const int &flight_frame_count,
      const VkDeviceSize &buffer_size
    );
    static void destroyDescriptorPool();
  private:
    Descriptor();

    inline static VkDevice *m_logical_device = nullptr;
    inline static std::optional<VkDescriptorSetLayout> *m_descriptor_set_layout = 
      nullptr;
    inline static VkDescriptorPool *m_descriptor_pool = nullptr;
  };
}