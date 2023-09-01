-- clean.lua

require "script/global"

newaction {
  trigger = "clean",
  description = "Remove generated files/folders",

  newoption {
    trigger = "make",
    value = "workspace",
    description = "Clean make files",
    allowed = {
      {"all", "all workspaces"}
    }
  },

  newoption {
    trigger = "build",
    value = "project",
    description = "Clean build(s)",
    allowed = {
      {"vkw", "vkw: vkw lib"},
      {"triangle", "triangle: project"},
      {"rectangle", "rectangle: project"}
    }
  },

  newoption {
    trigger = "bin",
    description = "binaries",
    description = "Clean binaries",
    allowed = {
      {"vkw", "vkw: binaries"},
      {"triangle", "triangle: binaries"},
      {"rectangle", "rectangle: binaries"}
    }
  },

  newoption {
    trigger = "all",
    description = "All options"
  },

  execute = function()
    if (_OPTIONS["make"] == "all") then
      print "Deleting /make"
      os.rmdir(mL)
    elseif (_OPTIONS["build"] == "vkw") then
      print "Deleting /build/vkw"
      os.rmdir(tL .. vkwL)
    elseif (_OPTIONS["build"] == "triangle") then
      print "Deleting /build/HelloTriangle"
      os.rmdir(tL .. helloTriangleL)
    elseif (_OPTIONS["build"] == "rectangle") then
      print "Deleting /build/HelloRectangle"
      os.rmdir(tL .. helloRectangleL)
    elseif (_OPTIONS["bin"] == "vkw") then
      print "Deleting /bin/vkw"
      os.rmdir(bL .. vkwL)
    elseif (_OPTIONS["bin"] == "triangle") then
      print "Deleting /bin/HelloTriangle"
      os.rmdir(bL .. helloTriangleL)
    elseif (_OPTIONS["bin"] == "rectangle") then
      print "Deleting /bin/HelloRectangle"
      os.rmdir(bL .. helloRectangleL)
    elseif (_OPTIONS["all"]) then
      print "Deleting /make, /build, and /bin"
      os.rmdir(mL)
      os.rmdir(tL)
      os.rmdir(bL)
    else 
      print "Invalid option"
    end
  end
}