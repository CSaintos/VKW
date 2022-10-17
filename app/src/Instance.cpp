// Instance.cpp
#include "Instance.hpp"

void vkw::Instance::createInstance
(
  VkInstance &vk_instance,
  const std::string &app_name,
  const int version[3],
  const char **extensions,
  const uint32_t &extension_count
)
{
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
  m_create_info.enabledExtensionCount = extension_count;
  m_create_info.ppEnabledExtensionNames = extensions;
  m_create_info.enabledLayerCount = 0;
  // Create vk instance
  if (vkCreateInstance(&m_create_info, nullptr, &vk_instance) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create instance!");
  }
}