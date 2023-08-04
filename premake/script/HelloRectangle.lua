-- HelloRectangle.lua
project "HelloRectangle"
  kind "WindowedApp"
  language "C++"
  location "%{wks.location}/%{prj.name}"
  targetdir "%{wks.location}/../build/%{prj.name}"
  objdir "%{wks.location}/../bin/%{prj.name}"

  files {
    "%{wks.location}/../app/test/HelloRectangle/**.cpp"
  }

  include_glm()

  use_vkw()
  use_GLFW()
  use_vulkan()

  includedirs {
    "%{wks.location}/../app/test/HelloRectangle"
  }

  postbuildcommands {
    "glslc %{wks.location}/../app/test/HelloRectangle/Base.vert -o %{wks.location}/../build/HelloRectangle/vert.spv",
    "glslc %{wks.location}/../app/test/HelloRectangle/Base.frag -o %{wks.location}/../build/HelloRectangle/frag.spv"
  }