// Validation.hpp
#pragma once

#include <vulkan\vulkan.hpp>

#include <cstdint>
#include <cstring>

#include <iostream>
#include <vector>

#include "vkw\Context.hpp"

namespace vkw
{
  /**
   * @class Validation
   * @brief handles error checking for vulkan api
  */
  class Validation
  {
  public:
    
    /// @brief  vector of validation layers
    static inline const std::vector<const char*> validation_layers = {
      "VK_LAYER_KHRONOS_validation"
    };

    /// @brief toggle for validation layers
    static const bool enable_validation_layers = true;
    
    /**
     * @brief Verifies if validation layers are available
     * 
     * @return true if available, false otherwise
    */
    static bool checkValidationLayerSupport();

    static void populateDebugMessengerCreateInfo
    (VkDebugUtilsMessengerCreateInfoEXT &create_info);

    static void setupDebugMessenger
    (
      Context &context
    );

    static void destroyDebugUtilsMessengerEXT
    (
      const VkAllocationCallbacks *p_allocator = nullptr
    );

  private:
    inline static VkDebugUtilsMessengerCreateInfoEXT m_create_info{};
    inline static VkInstance *m_vk_instance = nullptr;
    inline static VkDebugUtilsMessengerEXT *m_debug_messenger = nullptr;

    /**
     * @brief Validation constructor hidden
    */
    Validation();

    /**
     * @brief Vulkan debug calback method used by Vulkan
     * 
     * We specify our own debug callback method which Vulkan will
     * use to log debug messages.
    */
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback
    (
      VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
      VkDebugUtilsMessageTypeFlagsEXT message_type,
      const VkDebugUtilsMessengerCallbackDataEXT *p_callback_data,
      void *p_user_data
    );

    static VkResult createDebugUtilsMessengerEXT
    (
      const VkDebugUtilsMessengerCreateInfoEXT *p_create_info,
      const VkAllocationCallbacks *p_allocator = nullptr
    );

  };
}