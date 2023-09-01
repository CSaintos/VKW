-- descriptor.lua

require "script/global"
require "script/libs"

project "descriptor"
  kind "StaticLib"
  language "C++"
  initPrjLocs2()
  targetname("%{wks.name}")

  files {
    (wGL .. "/app/src/Descriptor.cpp")
  }

  includedirs {
    (wGL .. "/app/inc")
  }

  includeVulkan()

project "descriptor"