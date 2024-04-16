project "Grafika2D"
    kind "StaticLib"
    language "C++"
    staticruntime "on"
    cppdialect "C++17"

    targetdir ("%{wks.location}/bin/" .. outdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outdir .. "/%{prj.name}")

    files
    {
        "src/**.cpp",
        "src/**.c",
        "src/**.h"
    }

    includedirs
    {
        "src"
    }

    defines
    {
        
    }

    flags { "NoPCH" }

    filter "system:windows"
        systemversion "latest"
        links
        {
            "Dwrite.lib",
            "d2d1.lib",
            "Windowscodecs.lib",
            "Winmm.lib"
        }

    filter "configurations:Debug"
        defines "GRAFIKA_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "GRAFIKA_RELEASE"
        runtime "Release"
        optimize "on"