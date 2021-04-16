workspace "helloworlddynamic"
    configurations { "Debug", "Release" }
    platforms { "x64" }

project "helloworlddynamic"
    kind "SharedLib"
    language "C++"
    
    targetdir "../lib/%{cfg.buildcfg}"

    filter { "system:Windows" }
        files { "./src/*.h", "./src/*.cpp", "./src/*.def"  }

    filter { "system:Linux" }
        files { "./src/*.h", "./src/*.cpp" }

    filter { "configurations:Debug" }
        defines { "DEBUG" }
        symbols "On"

    filter { "configurations:Release" }
        defines { "NDEBUG" }
        optimize "On"
    
    filter { "system:Windows" }
    postbuildcommands {
        "{COPY} ../lib/%{cfg.buildcfg}/helloworlddynamic.dll ../helloworldexe/bin/%{cfg.buildcfg}/"
        }
        