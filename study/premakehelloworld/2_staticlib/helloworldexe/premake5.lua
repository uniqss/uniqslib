workspace "2_staticlib"
    configurations { "Debug", "Release" }
    platforms { "x64" }

project "helloworldexe"
    kind "ConsoleApp"
    language "C++"
    
    includedirs { "../mathutility_static/src" }

    libdirs { "../lib/%{cfg.buildcfg}" }

    links { "helloworldstatic" }

    targetdir "bin/%{cfg.buildcfg}"

    files { "./src/*.h", "./src/*.cpp" }

    filter { "configurations:Debug" }
        defines { "DEBUG" }
        symbols "On"

    filter { "configurations:Release" }
        defines { "NDEBUG" }
        optimize "On"

        