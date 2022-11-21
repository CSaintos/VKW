-- libs.lua

function include_vulkan()
  
  includedirs {
    "%{wks.location}/../lib/Vulkan/Include"
  }
end

function link_vulkan()
  
  libdirs {
    "%{wks.location}/../lib/Vulkan/Lib"
  }

  links {
    "vulkan-1"
  }
end

function use_vulkan()
  include_vulkan()
  link_vulkan()
end

function use_vkw()

  includedirs {
    "%{wks.location}/../build/vkw/inc"
  }

  libdirs {
    "%{wks.location}/../build/vkw/lib"
  }

  links {
    "vkw"
  }

end

function use_GLFW()

  includedirs {
    "%{wks.location}/../app/test/lib/GLFW"
  }

  libdirs {
    "%{wks.location}/../app/test/lib/GLFW"
  }

  links {
    "libglfw3",
    "gdi32"
  }

end

function include_glm()

  includedirs {
    "%{wks.location/../app/test/lib/glm"
  }
end