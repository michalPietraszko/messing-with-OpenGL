-- premake5.lua
workspace "OpenGL"
    architecture "x64"
    configurations { "Debug", "Release" }

IncludeDir = {}
IncludeDir["GLFW"] = "vendor/GLFW/include"
IncludeDir["GLEW"] = "vendor/GLEW/include"

include "/vendor/GLFW"

project "OpenApp"
    kind "ConsoleApp"
    language "C++"
    targetdir ("bin/%{cfg.buildcfg}")
    objdir ("bin-int/%{cfg.buildcfg}")
    cppdialect "C++17"

    files {"**.hpp", "src/**.cpp"}

    includedirs 
    {
        "src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLEW}"
    }

    libdirs
    { 
        "vendor/GLFW/bin/%{cfg.buildcfg}"
    }
    
    links
    {
        "GLFW.lib",
        "opengl32.lib"
    }
    
    filter "windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"
    
    filter "configurations:Debug"
        defines {"DEBUG"}
        symbols "On"

    filter "configurations:Release"
        defines {"NDEBUG"}
        optimize "On"
