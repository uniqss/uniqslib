workspace "helloworldstatic"
    configurations { "Debug", "Release" }
    platforms { "x64" }

project "helloworldstatic"
    kind "StaticLib"
    language "C++"
    
    targetdir "../lib/%{cfg.buildcfg}"

    files { "./src/*.h", "./src/*.cpp" }

    filter { "configurations:Debug" }
        defines { "DEBUG" }
        symbols "On"

    filter { "configurations:Release" }
        defines { "NDEBUG" }
        optimize "On"

        