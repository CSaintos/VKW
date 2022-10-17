// Instance.hpp
#pragma once

#include <vulkan\vulkan.hpp>

#include <cstdint>

#include <string>

namespace vkw
{
  /**
   * @class Instance
   * @brief Provides static methods for vk instance creation.
  */
  class Instance
  {
  public:
    /**
     * @brief Create vk instance
     * 
     * @param vk_instance Vulkan instance reference
     * 
     */
    static void createInstance
    (
      VkInstance &vk_instance,
      const std::string &app_name,
      const int version[3],
      const char **extensions = nullptr,
      const uint32_t &extension_count = 0
    );

  private:
    /// @brief Info about app
    inline static VkApplicationInfo m_app_info{};
    /// @brief Instance creation info
    inline static VkInstanceCreateInfo m_create_info{};

    inline static std::string m_engine_name = "";

    /**
     * @brief Instance constructor hiden
     */
    Instance();
  };
}