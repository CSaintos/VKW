-- physicalDevice.lua

require "script/global"
require "script/libs"

project "physicalDevice"
  kind "StaticLib"
  language "C++"
  initPrjLocs2()
  targetname("%{wks.name}")

  files {
    (wGL .. "/app/src/PhysicalDevice.cpp")
  }

  includedirs {
    (wGL .. "/app/inc")
  }

  includeVulkan()

project "physicalDevice"