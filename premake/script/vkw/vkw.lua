-- vkw.lua

require "script/global"

workspace "vkw"
  configurations { "Debug", "Release" }
  location (wML .. "/vkw")

  filter { "configurations:Debug" }
    defines { "DEBUG" }
    symbols "On"

  filter { "configurations:Release" }
    defines { "RELEASE" }
    optimize "On"

  filter {}

  include "script/vkw/validation.lua"
  include "script/vkw/instance.lua"
  include "script/vkw/queueFamilyIndices.lua"
  include "script/vkw/frameBuffer.lua"
  include "script/vkw/swapchain.lua"
  include "script/vkw/physicalDevice.lua"
  include "script/vkw/logicalDevice.lua"
  include "script/vkw/renderPass.lua"
  include "script/vkw/synchronization.lua"
  include "script/vkw/descriptor.lua"
  include "script/vkw/buffer.lua"
  include "script/vkw/command.lua"
  include "script/vkw/graphicsPipeline.lua"
  include "script/vkw/presentation.lua"

workspace "vkw"