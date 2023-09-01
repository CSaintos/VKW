-- main.lua

require "script/global"
require "script/libs"

project "HelloTriangle"
  kind "WindowedApp"
  language "C++"
  initPrjLocs()

  includeVulkan()
  includeGLFW()
  includeGLM()
  includeVkw()

  includedirs {
    (wGL .. "/app/test/HelloTriangle")
  }

  -- LINK LIBRARY ORDER MATTERS! from dependent to independent priority (don't forget)
  -- just the links portion: "-l" not "-L"
  useVkw()
  useGLFW()
  useVulkanLib()

  files {
    (wGL .. "/app/test/HelloTriangle/HelloTriangle.cpp")
  }

  postbuildcommands {
    ("glslc "..wGL.."/app/test/HelloTriangle/Base.vert -o "..wGL.."/build/HelloTriangle/vert.spv"),
    ("glslc "..wGL.."/app/test/HelloTriangle/Base.frag -o "..wGL.."/build/HelloTriangle/frag.spv")
  }

project "HelloTriangle"