-- premake5.lua
workspace "OpenGL"
    architecture "x64"
    configurations { "Debug", "Release" }
    location "build"

project "OpenApp"
    kind "ConsoleApp"
    language "C++"
    targetdir "bin/%{cfg.buildcfg}"
    cppdialect "C++17"

    files {"**.hpp", "src/**.cpp"}

    filter "configurations:Debug"
        defines {"DEBUG"}
        symbols "On"

    filter "configurations:Release"
        defines {"NDEBUG"}
        optimize "On"