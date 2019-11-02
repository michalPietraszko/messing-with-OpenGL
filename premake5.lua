-- premake5.lua
workspace "OpenGL"
    architecture "x64"
    configurations { "Debug", "Release" }
    location "build"

IncludeDir = {}
IncludeDir["GLFW"] = "vendor/GLFW/include"
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
        "%{IncludeDir.GLFW}"
    }

    libdirs{ "vendor/GLFW/src"}

    links
    {
        "glfw",
        "OpenGL.framework"
    }

    filter "configurations:Debug"
        defines {"DEBUG"}
        symbols "On"

    filter "configurations:Release"
        defines {"NDEBUG"}
        optimize "On"