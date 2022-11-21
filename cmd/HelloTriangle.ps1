
Set-Variable -Name "includes" -Value "-Iapp\test\lib\GLFW", `
"-Iapp\test\lib\glm", "-Ilib\Vulkan\Include", "-Ibuild\vkw\inc", `
"-Iapp\test\HelloTriangle"
Set-Variable -Name "links" -Value "-Llib\Vulkan\Lib", "-Lapp\test\lib\GLFW", `
"-Lbuild\vkw\lib", "-lvkw", "-llibglfw3", "-lgdi32", "-lvulkan-1"
Set-Variable -Name "defines" -Value ""

echo "clean"
if (Test-Path -Path "build\HelloTriangle\HelloTriangle.exe" -PathType Leaf) {
  rm build\HelloTriangle -Recurse
}

echo "compile"
g++ $includes -c app\test\HelloTriangle\HelloTriangle.cpp -o bin\HelloTriangle.o -g
# g++ $includes -c app\test\HelloTriangle\Presentation.cpp -o bin\Presentation.o -g

echo "build"
if (-not(Test-Path -Path "build\HelloTriangle" -PathType Container)) {
  New-Item -ItemType Directory -Path "build\HelloTriangle" | Out-Null
}

g++ bin\HelloTriangle.o $links `
-o build\HelloTriangle\HelloTriangle.exe -g

echo "compile shaders"
glslc app\test\HelloTriangle\Base.vert -o build\HelloTriangle\vert.spv
glslc app\test\HelloTriangle\Base.frag -o build\HelloTriangle\frag.spv

echo "obj-clean"
rm bin\*.o
