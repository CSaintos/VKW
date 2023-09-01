-- validation.lua

require "script/global"
require "script/libs"

project "validation"
  kind "StaticLib"
  language "C++"
  initPrjLocs2()
  targetname("%{wks.name}")

  files {
    (wGL .. "/app/src/Validation.cpp")
  }

  includedirs {
    (wGL .. "/app/inc")
  }

  includeVulkan()

project "validation"