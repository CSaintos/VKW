-- main.lua

require "script/global"
require "script/libs"

project "HelloRectangle"
  kind "WindowedApp"
  language "C++"
  initPrjLocs()

  files {
    (wGL .. "/app/test/HelloRectangle/HelloRectangle.cpp")
  }

  includeVulkan()
  includeGLFW()
  includeGLM()
  includeVkw()

  includedirs {
    (wGL .. "/app/test/HelloRectangle")
  }

  useVkw()
  useGLFW()
  useVulkanLib()

  postbuildcommands {
    ("glslc "..wGL.."/app/test/HelloRectangle/Base.vert -o "..wGL.."/build/HelloRectangle/vert.spv"),
    ("glslc "..wGL.."/app/test/HelloRectangle/Base.frag -o "..wGL.."/build/HelloRectangle/frag.spv")
  }

project "HelloRectangle"