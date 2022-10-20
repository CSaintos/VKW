// Instance.cpp
#include "vkw\Instance.hpp"

void vkw::Instance::createInstance
(
  VkInstance *vk_instance,
  const std::string &app_name,
  const int (&version)[3],
  const std::vector<const char*> &extensions
)
{
  // check for validation layers if validation is enabled
  if (Validation::enable_validation_layers && !Validation::checkValidationLayerSupport())
  {
    throw std::runtime_error("validation layers requested, but not available!");
  }
  // set engine name
  m_engine_name = app_name + " Engine";
  // Define app info
  m_app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  m_app_info.pApplicationName = app_name.c_str();
  m_app_info.applicationVersion = VK_MAKE_API_VERSION(0, version[0], version[1], version[2]);
  m_app_info.pEngineName = m_engine_name.c_str();
  m_app_info.engineVersion = VK_MAKE_API_VERSION(0, version[0], version[1], version[2]);
  m_app_info.apiVersion = VK_API_VERSION_1_0;
  // Define instance info
  m_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  m_create_info.pApplicationInfo = &m_app_info;
  m_create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  m_create_info.ppEnabledExtensionNames = extensions.data();
  
  // pass an additional debug message creation to Instance, to handle 
  // error checking in create and destroy instance
  VkDebugUtilsMessengerCreateInfoEXT debug_create_info{};
  if (Validation::enable_validation_layers)
  {
    m_create_info.enabledLayerCount = static_cast<uint32_t>(Validation::validation_layers.size());
    m_create_info.ppEnabledLayerNames = Validation::validation_layers.data();

    Validation::populateDebugMessengerCreateInfo(debug_create_info);
    m_create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debug_create_info;
  }
  else
  {
    m_create_info.enabledLayerCount = 0;

    m_create_info.pNext = nullptr;
  }

  // Create vk instance
  if (vkCreateInstance(&m_create_info, nullptr, vk_instance) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create instance!");
  }

  // Copy reference
  m_vk_instance = vk_instance;
}

void vkw::Instance::destroyInstance()
{
  vkDestroyInstance(*m_vk_instance, nullptr);
}