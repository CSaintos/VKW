-- frameBuffer.lua

require "script/global"
require "script/libs"

project "frameBuffer"
  kind "StaticLib"
  language "C++"
  initPrjLocs2()
  targetname("%{wks.name}")

  files {
    (wGL .. "/app/src/FrameBuffer.cpp")
  }

  includedirs {
    (wGL .. "/app/inc")
  }

  includeVulkan()

project "frameBuffer"