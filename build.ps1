git clone https://github.com/DincaG/Sfml-Framework
ren Sfml-Framework Framework
cmake ./
$projectName = "VizualizatorMatematic"

# because cmake devs in a nutshell https://gitlab.kitware.com/cmake/cmake/-/issues/25327
######################  patch  #########################
$vcxproj = Get-Content "$projectName.vcxproj" -Raw
$vcxproj = $vcxproj -replace 'xmlns="http://schemas.microsoft.com/developer/msbuild/2003">', @"
xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <PropertyGroup Condition="'`$(Configuration)|`$(Platform)'=='Debug|x64'" Label="Configuration">
    <UseDebugLibraries>true</UseDebugLibraries>
  </PropertyGroup>
  <PropertyGroup Condition="'`$(Configuration)|`$(Platform)'=='Release|x64'" Label="Configuration">
    <UseDebugLibraries>false</UseDebugLibraries>
  </PropertyGroup>
"@

$vcxproj | Set-Content "$projectName.vcxproj" -Encoding utf8



#####################  cleanup  ########################
$solutionContent = Get-Content "$projectName.sln" -Raw

$id = [regex]::matches($solutionContent, '(?<="ALL_BUILD.vcxproj", "){.*}').Value

$solutionContent = $solutionContent -replace '(Project[\s\S]*?"ALL_BUILD"[\s\S]*?EndProject)\s', ""
$solutionContent = $solutionContent -replace "(\t\t$id.*?\n)", ""

$solutionContent | Set-Content "$projectName.sln" -Encoding utf8

del .\cmake_install.cmake
del .\CMakeFiles -Recurse
del .\ALL_BUILD.vcxproj
del .\ALL_BUILD.vcxproj.filters
del .\CMakeCache.txt