workspace "2_dynamiclib"
    configurations { "Debug", "Release" }
    platforms { "x64" }

project "helloworldexe"
    kind "ConsoleApp"
    language "C++"
    
    includedirs { "../mathutility_dynamic/src" }

    libdirs { "../lib/%{cfg.buildcfg}" }

    links { "helloworlddynamic" }

    targetdir "bin/%{cfg.buildcfg}"

    files { "./src/*.h", "./src/*.cpp" }

    filter { "configurations:Debug" }
        defines { "DEBUG" }
        symbols "On"

    filter { "configurations:Release" }
        defines { "NDEBUG" }
        optimize "On"

        