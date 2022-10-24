
Set-Variable -Name "includes" -Value "-Iapp\test\lib\GLFW", `
"-Iapp\test\lib\glm", "-Ilib\Vulkan\Include", "-Ibuild\vkw\inc"
Set-Variable -Name "links" -Value "-Llib\Vulkan\Lib", "-Lapp\test\lib\GLFW", `
"-Lbuild\vkw\lib", "-lvkw", "-l:libglfw3.a", "-lgdi32", "-lvulkan-1"
Set-Variable -Name "defines" -Value ""

echo "clean"
if (Test-Path -Path "build\HelloTriangle\HelloTriangle.exe" -PathType Leaf) {
  rm build\HelloTriangle -Recurse
}

echo "compile"
g++ $includes -c app\test\HelloTriangle\HelloTriangle.cpp -o bin\helloTriangle.o -g

echo "build"
if (-not(Test-Path -Path "build\HelloTriangle" -PathType Container)) {
  New-Item -ItemType Directory -Path "build\HelloTriangle" | Out-Null
}

g++ bin\helloTriangle.o $links -o build\HelloTriangle\HelloTriangle.exe -g

echo "compile shaders"
glslc app\test\HelloTriangle\Base.vert -o build\HelloTriangle\vert.spv
glslc app\test\HelloTriangle\Base.frag -o build\HelloTriangle\frag.spv

echo "obj-clean"
rm bin\*.o
