// Instance.hpp
#pragma once

#include <vulkan\vulkan.hpp>

#include <cstdint>

#include <string>
#include <vector>

#include "vkw\Validation.hpp"

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
     * @param app_name Name of app string
     * @param version Version of app int[3]
     * @param extensions Extensions for Vulkan (optional)
     * -@param extension_count Number of extensions provided (optional)
     */
    static void createInstance
    (
      VkInstance *vk_instance,
      const std::string &app_name,
      const int (&version)[3],
      const std::vector<const char*> &extensions = {}
      //const char **extensions = nullptr,
      // const uint32_t &extension_count = 0
    );

    /**
     * @brief Destroy vk instance
    */
    static void destroyInstance();

  private:
    /// @brief Info about app
    inline static VkApplicationInfo m_app_info{};
    /// @brief Instance creation info
    inline static VkInstanceCreateInfo m_create_info{};
    
    inline static std::string m_engine_name = "";

    inline static VkInstance *m_vk_instance = nullptr;

    /**
     * @brief Instance constructor hidden
     */
    Instance();
  };
}