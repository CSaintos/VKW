-- VKW.lua
project "vkw"
  kind "StaticLib"
  language "C++"
  location "%{wks.location}/%{prj.name}"
  targetdir "%{wks.location}/../build/%{prj.name}/lib"
  objdir "%{wks.location}/../bin/%{prj.name}"

  files {
    "%{wks.location}/../app/src/**.cpp"
  }

  includedirs {
    "%{wks.location}/../app/inc",
    "%{wks.location}/../app/inc/**"
  }

  include_vulkan()

  postbuildcommands {
    "{COPYDIR} %{wks.location}/../app/inc %{cfg.buildtarget.directory}/../inc"
  }