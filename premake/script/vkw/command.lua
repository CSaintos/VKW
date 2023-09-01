-- command.lua

require "script/global"
require "script/libs"

project "command"
  kind "StaticLib"
  language "C++"
  initPrjLocs2()
  targetname("%{wks.name}")


  files {
    (wGL .. "/app/src/Command.cpp")
  }

  includedirs {
    (wGL .. "/app/inc")
  }

  includeVulkan()

project "command"