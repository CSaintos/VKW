
Set-Variable -Name "includes" -Value "-Iapp\inc", "-Ilib\Vulkan\Include"
#Set-Variable -Name "links" -Value "-Llib\Vulkan\Lib", "-lvulkan-1"
Set-Variable -Name "defines" -Value ""

echo "clean"
if (Test-Path -Path "build\vkw" -PathType Container) {
  rm build\vkw -Recurse
}

#echo "clean package"
#if (Test-Path -Path "app\test\lib\vkw" -PathType Container) {
#  rm app\test\lib\vkw\* -Recurse
#}

echo "compile"
g++ $includes -c app\src\Instance.cpp -o bin\Instance.o -g
g++ $includes -c app\src\Validation.cpp -o bin\Validation.o -g
g++ $includes -c app\src\QueueFamilyIndices.cpp -o bin\QueueFamilyIndices.o -g
g++ $includes -c app\src\PhysicalDevice.cpp -o bin\PhysicalDevice.o -g
g++ $includes -c app\src\LogicalDevice.cpp -o bin\LogicalDevice.o -g
g++ $includes -c app\src\SwapChain.cpp -o bin\SwapChain.o -g
g++ $includes -c app\src\RenderPass.cpp -o bin\RenderPass.o -g
g++ $includes -c app\src\GraphicsPipeline.cpp -o bin\GraphicsPipeline.o -g
g++ $includes -c app\src\Framebuffer.cpp -o bin\Framebuffer.o -g
g++ $includes -c app\src\Command.cpp -o bin\Command.o -g
g++ $includes -c app\src\Synchronization.cpp -o bin\Synchronization.o -g
g++ $includes -c app\src\Presentation.cpp -o bin\Presentation.o -g

echo "build"

if (-not(Test-Path -Path "build\vkw" -PathType Container)) {
  New-Item -ItemType Directory -Path "build\vkw" | Out-Null
}
if (-not(Test-Path -Path "build\vkw\inc" -PathType Container)) {
  New-Item -ItemType Directory -Path "build\vkw\inc" | Out-Null
}
if (-not(Test-Path -Path "build\vkw\lib" -PathType Container)) {
  New-Item -ItemType Directory -Path "build\vkw\lib" | Out-Null
}

ar -q build\vkw\lib\vkw.lib bin\Validation.o bin\Instance.o bin\QueueFamilyIndices.o `
bin\PhysicalDevice.o bin\LogicalDevice.o bin\SwapChain.o bin\RenderPass.o `
bin\GraphicsPipeline.o bin\Framebuffer.o bin\Command.o bin\Synchronization.o `
bin\Presentation.o
Copy-Item -Path "app\inc\*" -Destination "build\vkw\inc" -Recurse

echo "obj-clean"
rm bin\*.o

#echo "package"
#Copy-Item -Path "build\vkw\*" -Destination "app\test\lib\vkw" -Recurse