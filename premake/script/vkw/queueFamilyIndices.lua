-- queueFamilyIndices.lua

require "script/global"
require "script/libs"

project "queueFamilyIndices"
  kind "StaticLib"
  language "C++"
  initPrjLocs2()
  targetname("%{wks.name}")

  files {
    (wGL .. "/app/src/QueueFamilyIndices.cpp")
  }

  includedirs {
    (wGL .. "/app/inc")
  }

  includeVulkan()

project "queueFamilyIndices"