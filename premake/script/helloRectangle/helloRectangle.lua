-- helloRectangle.lua

require "script/global"

workspace "helloRectangle"
  configurations { "Debug", "Release" }
  location (wML .. "/helloRectangle")

  filter { "configurations:Debug" }
    defines { "DEBUG" }
    symbols "On"

  filter { "configurations:Release" }
    defines { "RELEASE" }
    optimize "On"

  filter {}

  include "script/helloRectangle/main.lua"

workspace "helloRectangle"