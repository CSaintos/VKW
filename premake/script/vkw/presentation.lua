-- presentation.lua

require "script/global"
require "script/libs"

project "presentation"
  kind "StaticLib"
  language "C++"
  initPrjLocs2()
  targetname("%{wks.name}")

  files {
    (wGL .. "/app/src/Presentation.cpp")
  }

  includedirs {
    (wGL .. "/app/inc")
  }

  includeVulkan()

project "presentation"