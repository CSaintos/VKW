-- build.lua

require "script/global"

newaction {
  trigger = "build",
  description = "Build project (mingw32-make)",

  newoption {
    trigger = "vkw",
    value = "project",
    description = "Build vkw lib",
    allowed = {
      {"all", "all vkw project"},
      {"buffer", "buffer lib"},
      {"command", "command lib"},
      {"descriptor", "descriptor lib"},
      {"frameBuffer", "frameBuffer lib"},
      {"graphicsPipeline", "graphicsPipeline lib"},
      {"instance", "instance lib"},
      {"logicalDevice", "logicalDevice lib"},
      {"main", "main lib"},
      {"physicalDevice", "physicalDevice lib"},
      {"presentation", "presentation lib"},
      {"queueFamilyIndices", "queueFamilyIndices lib"},
      {"renderPass", "renderPass lib"},
      {"swapchain", "swapchain lib"},
      {"synchronization", "synchronization lib"},
      {"validation", "validation lib"}
    }
  },

  newoption {
    trigger = "triangle",
    value = "sample proj",
    description = "Build HelloTriangle",
    allowed = {
      {"all", "Entire project"}
    }
  },

  newoption {
    trigger = "rectangle",
    value = "sample proj",
    description = "Build HelloRectangle",
    allowed = {
      {"all", "Entire project"}
    }
  },

  execute = function()
    if (_OPTIONS["vkw"] == "all") then
      print "Building vkw dll"
      --makeBuild(mL .. vkwL)
      copyDir("/app/inc", "/build/vkw/inc")
    elseif (_OPTIONS["vkw"] == "buffer") then
      print "Building buffer lib"
      makeBuild(mL .. bufferL)
    elseif (_OPTIONS["vkw"] == "command") then
      print "Building command lib"
      makeBuild(mL .. commandL)
    elseif (_OPTIONS["vkw"] == "descriptor") then
      print "Building descriptor lib"
      makeBuild(mL .. descriptorL)
    elseif (_OPTIONS["vkw"] == "frameBuffer") then
      print "Building frameBuffer lib"
      makeBuild(mL .. frameBufferL)
    elseif (_OPTIONS["vkw"] == "graphicsPipeline") then
      print "Building graphicsPipeline lib"
      makeBuild(mL .. graphicsPipelineL)
    elseif (_OPTIONS["vkw"] == "instance") then
      print "Building instance lib"
      makeBuild(mL .. instanceL)
    elseif (_OPTIONS["vkw"] == "logicalDevice") then
      print "Building logicalDevice lib"
      makeBuild(mL .. logicalDeviceL)
    elseif (_OPTIONS["vkw"] == "physicalDevice") then
      print "Building physicalDevice lib"
      makeBuild(mL .. physicalDeviceL)
    elseif (_OPTIONS["vkw"] == "presentation") then
      print "Building presentation lib"
      makeBuild(mL .. presentationL)
    elseif (_OPTIONS["vkw"] == "queueFamilyIndices") then
      print "Building queueFamilyIndices lib"
      makeBuild(mL .. queueFamilyIndicesL)
    elseif (_OPTIONS["vkw"] == "renderPass") then
      print "Building renderPass lib"
      makeBuild(mL .. renderPassL)
    elseif (_OPTIONS["vkw"] == "swapchain") then
      print "Building swapchain lib"
      makeBuild(mL .. swapchainL)
    elseif (_OPTIONS["vkw"] == "synchronization") then
      print "Building synchronization lib"
      makeBuild(mL .. synchronizationL)
    elseif (_OPTIONS["vkw"] == "validation") then
      print "Building validation lib"
      makeBuild(mL .. validationL)
    elseif (_OPTIONS["triangle"] == "all") then
      print "Building HelloTriangle proj"
      makeBuild(mL .. helloTriangleL)
    elseif (_OPTIONS["rectangle"] == "all") then
      print "Building HelloRectangle proj"
      makeBuild(mL .. helloRectangleL)
    else
      print "Invalid option or argument"
    end
  end
}