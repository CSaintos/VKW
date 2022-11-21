-- commands.lua
newaction {
  trigger = "clean",
  description = "Clean workspace (build default)",

  newoption {
    trigger = "project",
    description = "Project files"
  },

  newoption {
    trigger = "build",
    description = "Build directory"
  },

  newoption {
    trigger = "bin",
    description = "Binaries"
  },

  newoption {
    trigger = "all",
    description = "All options"
  },

  onStart = function()
    -- evaluates options
    project = false
    build = false
    bin = false

    if (_OPTIONS["project"]) then
      project = true
    elseif (_OPTIONS["build"]) then
      build = true
    elseif (_OPTIONS["bin"]) then
      bin = true
    elseif (_OPTIONS["all"]) then
      project = true
      build = true
      bin = true
    else
      build = true
    end
  end,

  onWorkspace = function(wks)
    if (project) then
      local path = (wks.location .. "/Makefile")
      os.remove(path)
    end
  end,

  onProject = function(prj)
    if (project) then
      os.rmdir(prj.location)
    end
  end,

  execute = function()
    if (project) then
      os.rmdir("../make")
    end
    if (build) then
      os.rmdir("../build")
    end
    if (bin) then
      os.rmdir("../bin")
    end
  end
}

newaction {
  trigger = "build",
  description = "Build project (mingw32-make)",

  newoption {
    trigger = "vkw",
    description = "vkw project"
  },

  newoption {
    trigger = "HelloTriangle",
    description = "HelloTriangle project"
  },

  newoption {
    trigger = "HelloRectangle",
    description = "HelloRectangle project"
  },

  onStart = function()
    -- evaluates options
    vkw = false
    ht = false
    hr = false
    
    if (_OPTIONS["vkw"]) then
      vkw = true
    elseif (_OPTIONS["HelloTriangle"]) then
      ht = true
    elseif (_OPTIONS["HelloRectangle"]) then
      hr = true
    else
      vkw = true
      ht = true
      hr = true
    end
  end,

  onWorkspace = function(wks)
    os.chdir(wks.location)

    if (vkw) then
      os.execute("mingw32-make vkw")
    end
    if (ht) then
      os.execute("mingw32-make HelloTriangle")
    end
    if (hr) then
      os.execute("mingw32-make HelloRectangle")
    end
  end
}