-- HelloTriangle.lua
project "HelloTriangle"
  kind "WindowedApp"
  language "C++"
  location "%{wks.location}/%{prj.name}"
  targetdir "%{wks.location}/../build/%{prj.name}"
  objdir "%{wks.location}/../bin/%{prj.name}"

  files {
    "%{wks.location}/../app/test/HelloTriangle/**.cpp"
  }

  include_glm()

  use_vkw()
  use_GLFW()
  use_vulkan()

  postbuildcommands {
    "glslc %{wks.location}/../app/test/HelloTriangle/Base.vert -o %{wks.location}/../build/HelloTriangle/vert.spv",
    "glslc %{wks.location}/../app/test/HelloTriangle/Base.frag -o %{wks.location}/../build/HelloTriangle/frag.spv"
  }
