-- instance.lua

require "script/global"
require "script/libs"

project "instance"
  kind "StaticLib"
  language "C++"
  initPrjLocs2()
  targetname("%{wks.name}")

  files {
    (wGL .. "/app/src/Instance.cpp")
  }

  includedirs {
    (wGL .. "/app/inc")
  }

  includeVulkan()
project "instance"