
Set-Variable -Name "includes" -Value "-Iapp\test\lib\GLFW", `
"-Iapp\test\lib\glm", "-Ilib\Vulkan\Include", "-Ibuild\vkw\inc", `
"-Iapp\test\HelloRectangle"
Set-Variable -Name "links" -Value "-Llib\Vulkan\Lib", "-Lapp\test\lib\GLFW", `
"-Lbuild\vkw\lib", "-lvkw", "-l:libglfw3.a", "-lgdi32", "-lvulkan-1"
Set-Variable -Name "defines" -Value ""

echo "clean"
if (Test-Path -Path "build\HelloRectangle\HelloRectangle.exe" -PathType Leaf) {
  rm build\HelloRectangle -Recurse
}

echo "compile"
g++ $includes -c app\test\HelloRectangle\HelloRectangle.cpp `
-o bin\HelloRectangle.o -g

echo "build"
if (-not(Test-Path -Path "build\HelloRectangle" -PathType Container)) {
  New-Item -ItemType Directory -Path "build\HelloRectangle" | Out-Null
}

g++ bin\HelloRectangle.o $links `
-o build\HelloRectangle\HelloRectangle.exe -g

echo "compile shaders"
glslc app\test\HelloRectangle\Base.vert -o build\HelloRectangle\vert.spv
glslc app\test\HelloRectangle\Base.frag -o build\HelloRectangle\frag.spv

echo "obj-clean"
rm bin\*.o
