-- graphicsPipeline.lua

require "script/global"
require "script/libs"

project "graphicsPipeline"
  kind "StaticLib"
  language "C++"
  initPrjLocs2()
  targetname("%{wks.name}")

  files {
    (wGL .. "/app/src/GraphicsPipeline.cpp")
  }

  includedirs {
    (wGL .. "/app/inc")
  }

  includeVulkan()

project "graphicsPipeline"