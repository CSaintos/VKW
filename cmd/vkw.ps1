
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

ar -q build\vkw\lib\vkw.lib bin\Instance.o
Copy-Item -Path "app\inc\*" -Destination "build\vkw\inc" -Recurse

echo "obj-clean"
rm bin\*.o

#echo "package"
#Copy-Item -Path "build\vkw\*" -Destination "app\test\lib\vkw" -Recurse