// Validation.cpp
#include "vkw/Validation.hpp"

bool vkw::Validation::checkValidationLayerSupport()
{
  uint32_t layer_count;
  vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
  
  std::vector<VkLayerProperties> available_layers(layer_count);
  vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

  for (const char *layer_name : validation_layers)
  {
    bool layer_found = false;

    for (const VkLayerProperties &layer_properties : available_layers)
    {
      if (strcmp(layer_name, layer_properties.layerName) == 0)
      {
        layer_found = true;
        break;
      }
    }

    if (!layer_found)
    {
      return false;
    }
  }
  return true;
}

VkBool32 vkw::Validation::debugCallback
(
  VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
  VkDebugUtilsMessageTypeFlagsEXT message_type,
  const VkDebugUtilsMessengerCallbackDataEXT *p_callback_data,
  void *p_user_data
)
{
  std::cerr << "validation layer: " << p_callback_data->pMessage << std::endl;

  return VK_FALSE;
}

void vkw::Validation::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &create_info)
{
  create_info = {};
  create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  create_info.messageSeverity = 
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  create_info.messageType =
    VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  create_info.pfnUserCallback = debugCallback;
  create_info.pUserData = nullptr; // optional
}

void vkw::Validation::setupDebugMessenger
(
  Context &context
)
{
  // link static vars
  m_vk_instance = &context.instance;
  m_debug_messenger = &context.debug_messenger;

  if (!enable_validation_layers) return;

  // populate debug messenger create info
  populateDebugMessengerCreateInfo(m_create_info);

  if (createDebugUtilsMessengerEXT(&m_create_info) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to setup debug messenger!");
  }
}

VkResult vkw::Validation::createDebugUtilsMessengerEXT
(
  //VkInstance instance,
  const VkDebugUtilsMessengerCreateInfoEXT *p_create_info,
  const VkAllocationCallbacks *p_allocator
  //VkDebugUtilsMessengerEXT *p_debug_messenger
)
{
  auto func = 
    (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr
    (
      *m_vk_instance, "vkCreateDebugUtilsMessengerEXT"  
    );
  
  if (func != nullptr)
  {
    return func(*m_vk_instance, p_create_info, p_allocator, m_debug_messenger);
  }
  else
  {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

void vkw::Validation::destroyDebugUtilsMessengerEXT
(
  const VkAllocationCallbacks *p_allocator
)
{
  if (!enable_validation_layers) return; 

  auto func = 
    (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr
    (
      *m_vk_instance, "vkDestroyDebugUtilsMessengerEXT"
    );
  
  if (func != nullptr)
  {
    func(*m_vk_instance, *m_debug_messenger, p_allocator);
  }
}