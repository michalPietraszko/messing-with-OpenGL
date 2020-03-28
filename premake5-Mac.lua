-- premake5.lua

workspace "OpenGL"
    architecture "x64"
    configurations { "Debug", "Release" }
    location "build"
    toolset "clang"

IncludeDir = {}
IncludeDir["GLFW"] = "vendor/GLFW/include"
IncludeDir["GLEW"] = "vendor/GLEW/include"

--include premake file
--include "vendor/GLFW/"

project "OpenApp"
    location "build"
    kind "ConsoleApp"
    language "C++"
    targetdir "bin/%{cfg.buildcfg}"
    cppdialect "C++17"

    files {"**.hpp", "src/**.cpp"}
    includedirs 
    {
        "src",
        "%{IncludeDir.GLEW}",
        "%{IncludeDir.GLFW}"
    }

    libdirs
    { 
        "vendor/GLFW/src",
        "vendor/GLEW/lib/mac"
    }

    links
    {
        "GLEW",
        "glfw",
        "OpenGL.framework"
    }

    filter "configurations:Debug"
        defines {"DEBUG"}
        symbols "On"

    filter "configurations:Release"
        defines {"NDEBUG"}
        optimize "On"