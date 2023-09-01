
Set-Variable -Name "includes" -Value "-Iapp\inc", "-Ilib\Vulkan\Include"

echo "clean"
if (Test-Path -Path "build\vkw" -PathType Container) {
  rm build\vkw -Recurse
}

echo "compile"
if (-not(Test-Path -Path "bin" -PathType Container)) {
  New-Item -ItemType Directory -Path "bin" | Out-Null
}
g++ $includes -c app\src\Instance.cpp -o bin\Instance.o -g
g++ $includes -c app\src\Validation.cpp -o bin\Validation.o -g
g++ $includes -c app\src\QueueFamilyIndices.cpp -o bin\QueueFamilyIndices.o -g
g++ $includes -c app\src\PhysicalDevice.cpp -o bin\PhysicalDevice.o -g
g++ $includes -c app\src\LogicalDevice.cpp -o bin\LogicalDevice.o -g
g++ $includes -c app\src\Swapchain.cpp -o bin\Swapchain.o -g
g++ $includes -c app\src\RenderPass.cpp -o bin\RenderPass.o -g
g++ $includes -c app\src\GraphicsPipeline.cpp -o bin\GraphicsPipeline.o -g
g++ $includes -c app\src\Framebuffer.cpp -o bin\Framebuffer.o -g
g++ $includes -c app\src\Command.cpp -o bin\Command.o -g
g++ $includes -c app\src\Synchronization.cpp -o bin\Synchronization.o -g
g++ $includes -c app\src\Presentation.cpp -o bin\Presentation.o -g
g++ $includes -c app\src\Buffer.cpp -o bin\Buffer.o -g
g++ $includes -c app\src\Descriptor.cpp -o bin\Descriptor.o -g

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

ar -rcs build\vkw\lib\vkw.lib bin\Instance.o
ar -rs build\vkw\lib\vkw.lib bin\Validation.o
ar -rs build\vkw\lib\vkw.lib bin\QueuefamilyIndices.o
ar -rs build\vkw\lib\vkw.lib bin\PhysicalDevice.o
ar -rs build\vkw\lib\vkw.lib bin\LogicalDevice.o
ar -rs build\vkw\lib\vkw.lib bin\Swapchain.o
ar -rs build\vkw\lib\vkw.lib bin\RenderPass.o
ar -rs build\vkw\lib\vkw.lib bin\GraphicsPipeline.o
ar -rs build\vkw\lib\vkw.lib bin\Framebuffer.o
ar -rs build\vkw\lib\vkw.lib bin\Command.o
ar -rs build\vkw\lib\vkw.lib bin\Synchronization.o
ar -rs build\vkw\lib\vkw.lib bin\Presentation.o
ar -rs build\vkw\lib\vkw.lib bin\Buffer.o
ar -rs build\vkw\lib\vkw.lib bin\Descriptor.o

Copy-Item -Path "app\inc\*" -Destination "build\vkw\inc" -Recurse

echo "obj-clean"
rm bin\*.o