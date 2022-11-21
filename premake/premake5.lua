-- premake5.lua

workspace "Workspace"
  configurations { "Debug", "Release" }
  -- platforms { "Win64" }
  location "../make"

  filter { "platforms:Win64" }
    system "Windows"
    architecture "x64"
  
  filter { "configurations:Debug" }
    defines { "DEBUG" }
    symbols "On"
  
  filter { "configurations:Release" }
    defines { "RELEASE" }
    optimize "On"

require "script/commands"
require "script/libs"
require "script/vkw"
require "script/HelloTriangle"
require "script/HelloRectangle"