-- logicalDevice.lua

require "script/global"
require "script/libs"

project "logicalDevice"
  kind "StaticLib"
  language "C++"
  initPrjLocs2()
  targetname("%{wks.name}")

  files {
    (wGL .. "/app/src/LogicalDevice.cpp")
  }

  includedirs {
    (wGL .. "/app/inc")
  }

  includeVulkan()

project "logicalDevice"