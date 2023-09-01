-- helloTriangle.lua

require "script/global"

workspace "helloTriangle"
  configurations { "Debug", "Release" }
  location (wML .. "/helloTriangle")

  filter { "configurations:Debug" }
    defines { "DEBUG" }
    symbols "On"

  filter { "configurations:Release" }
    defines { "RELEASE" }
    optimize "On"

  filter {}

  include "script/helloTriangle/main.lua"

workspace "helloTriangle"