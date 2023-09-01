-- premake5.lua

-- workspaces
include "script/vkw/vkw.lua"
include "script/helloTriangle/helloTriangle.lua"
include "script/helloRectangle/helloRectangle.lua"

-- cmds
include "script/cmd/build.lua"
include "script/cmd/clean.lua"

-- workspace "Workspace"
--   configurations { "Debug", "Release" }
--   -- platforms { "Win64" }
--   location "../make"

--   filter { "platforms:Win64" }
--     system "Windows"
--     architecture "x64"
  
--   filter { "configurations:Debug" }
--     defines { "DEBUG" }
--     symbols "On"
  
--   filter { "configurations:Release" }
--     defines { "RELEASE" }
--     optimize "On"

-- require "script/commands"
-- require "script/libs"
-- require "script/vkw"
-- require "script/HelloTriangle"
-- require "script/HelloRectangle"