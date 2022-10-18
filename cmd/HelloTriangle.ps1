
Set-Variable -Name "includes" -Value "-Iapp\test\lib\GLFW", `
"-Iapp\test\lib\glm", "-Ilib\Vulkan\Include", "-Ibuild\vkw\inc"
Set-Variable -Name "links" -Value "-Llib\Vulkan\Lib", "-Lapp\test\lib\GLFW", `
"-Lbuild\vkw\lib", "-lvkw", "-l:libglfw3.a", "-lgdi32", "-lvulkan-1"
Set-Variable -Name "defines" -Value ""

echo "clean"
if (Test-Path -Path "build\HelloTriangle.exe" -PathType Leaf) {
  rm build\HelloTriangle.exe
}

echo "compile"
g++ $includes -c app\test\HelloTriangle.cpp -o bin\helloTriangle.o -g

echo "build"
g++ bin\helloTriangle.o $links -o build\HelloTriangle.exe -g

echo "obj-clean"
rm bin\*.o