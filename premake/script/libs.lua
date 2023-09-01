-- libs.lua

require "script/global"

function includeVulkan()
  includedirs {
    (wGL .. "/lib/Vulkan/Include")
  }
end

function useVulkanLib()
  libdirs {
    (wGL.."/lib/Vulkan/Lib")
  }
  links {
    "vulkan-1"
  }
end

function includeVkw()
  includedirs {
    (wGL.."/build/vkw/inc")
  }
end

function useVkw()
  libdirs {
    (wGL .. "/build/vkw/lib")
  }
  
  links {
    "vkw"
  }
end

function includeGLFW()
  includedirs {
    (wGL .. "/app/test/lib/GLFW")
  }
end

function useGLFW()
  libdirs {
    (wGL .. "/app/test/lib/GLFW")
  }

  links {
    "libglfw3",
    "gdi32"
  }
end


function includeGLM()
  includedirs {
    (wGL .. "/app/test/lib/glm")
  }
end