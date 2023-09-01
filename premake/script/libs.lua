-- libs.lua

require "script/global"

function includeVulkan()
  includedirs {
    (wGL .. "/lib/Vulkan/Include")
  }
end

--! deprecated
function include_vulkan()
  
  includedirs {
    "%{wks.location}/../lib/Vulkan/Include"
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

--! deprecated
function link_vulkan()
  
  libdirs {
    "%{wks.location}/../lib/Vulkan/Lib"
  }

  links {
    "vulkan-1"
  }
end

--! deprecated
function use_vulkan()
  include_vulkan()
  link_vulkan()
end

function useBufferLib()
  libdirs {
    (wGL .. "/build/vkw/lib")
  }
  links {
    ("buffer:static")
  }
end

function useCommandLib()
  libdirs {
    (wGL .. "/build/vkw/lib")
  }
  links {
    ("command:static")
  }
end

function useDescriptorLib()
  libdirs {
    (wGL .. "/build/vkw/lib")
  }
  links {
    ("descriptor:static")
  }
end

function useFrameBufferLib()
  libdirs {
    (wGL .. "/build/vkw/lib")
  }
  links {
    ("frameBuffer:static")
  }
end

function useGraphicsPipelineLib()
  libdirs {
    (wGL .. "/build/vkw/lib")
  }
  links {
    ("graphicsPipeline:static")
  }
end

function useInstanceLib()
  libdirs {
    (wGL .. "/build/vkw/lib")
  }
  links {
    ("instance:static")
  }
end

function useLogicalDeviceLib()
  libdirs {
    (wGL .. "/build/vkw/lib")
  }
  links {
    ("logicalDevice:static")
  }
end

function usePhysicalDeviceLib()
  libdirs {
    (wGL .. "/build/vkw/lib")
  }
  links {
    ("physicalDevice:static")
  }
end

function usePresentationLib()
  libdirs {
    (wGL .. "/build/vkw/lib")
  }
  links {
    ("presentation:static")
  }
end

function useQueueFamilyIndicesLib()
  libdirs {
    (wGL .. "/build/vkw/lib")
  }
  links {
    ("queueFamilyIndices:static")
  }
end

function useRenderPassLib()
  libdirs {
    (wGL .. "/build/vkw/lib")
  }
  links {
    ("renderPass:static")
  }
end

function useSwapchainLib()
  libdirs {
    (wGL .. "/build/vkw/lib")
  }
  links {
    ("swapchain:static")
  }
end

function useSynchronizationLib()
  libdirs {
    (wGL .. "/build/vkw/lib")
  }
  links {
    ("synchronization:static")
  }
end

function useValidationLib()
  libdirs {
    (wGL .. "/build/vkw/lib")
  }
  links {
    ("validation:static")
  }
end

--! deprecated
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

--! deprecated
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

--! deprecated
function include_glm()

  includedirs {
    "%{wks.location}/../app/test/lib/glm"
  }
end

function includeGLM()
  includedirs {
    (wGL .. "/app/test/lib/glm")
  }
end