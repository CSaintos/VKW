-- global.lua

-- OS specific vars
if os.host() == "windows" then
  makeCmd = "mingw32-make"
  soSuffix = ".dll"
  soPrefix = "/"
else 
  makeCmd = "make"
  soSuffix = ".so"
  soPrefix = "/lib"
end

-- paths
gL = ".."
mL = gL .. "/make"
tL = gL .. "/build"
bL = gL .. "/bin"

vkwL = "/vkw"
  bufferL = vkwL .. "/buffer"
  commandL = vkwL .. "/command"
  descriptorL = vkwL .. "/descriptor"
  frameBufferL = vkwL .. "/frameBuffer"
  graphicsPipelineL = vkwL .. "/graphicsPipeline"
  instanceL = vkwL .. "/instance"
  logicalDeviceL = vkwL .. "/logicalDevice"
  physicalDeviceL = vkwL .. "/physicalDevice"
  presentationL = vkwL .. "/presentation"
  queueFamilyIndicesL = vkwL .. "/queueFamilyIndices"
  renderPassL = vkwL .. "/renderPass"
  swapchainL = vkwL .. "/swapchain"
  synchronizationL = vkwL .. "/synchronization"
  validationL = vkwL .. "/validation"
  mainL = vkwL .. "/vkw"
helloRectangleL = "/HelloRectangle"
helloTriangleL = "/HelloTriangle"

-- workspace paths
wGL = "../../.."
wML = (wGL .. "/make")

-- project paths
prjL = "%{wks.location}/%{prj.name}"
tarL = "%{wks.location}/../../build/%{wks.name}/%{prj.name}"
objL = "%{wks.location}/../../bin/%{wks.name}/%{prj.name}"

-- project functions
function initPrjLocs()
  location (prjL)
  targetdir ("%{wks.location}/../../build/%{prj.name}")
  objdir ("%{wks.location}/../../bin/%{prj.name}")
end
function initPrjLocs2()
  location (prjL)
  targetdir ("%{wks.location}/../../build/%{wks.name}/lib")
  objdir (objL)
end

-- build functions
function copySO(fromDir, toDir, fileName)
  os.execute("{COPYFILE} "..tL..fromDir..soPrefix..fileName..soSuffix.." "..tL..toDir)
end
function copyDir(fromDir, toDir)
  os.execute("{COPYDIR} "..gL..fromDir.." "..gL..toDir)
end
function makeBuild(path)
  local cwd = os.getcwd()
  os.chdir(path)
  os.execute(makeCmd)
  os.chdir(cwd)
end