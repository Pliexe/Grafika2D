project "MyGame"
    kind "WindowedApp"
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
        "src",
        "%{wks.location}/Grafika2D/src",
        -- "%{IncludeDir.Yaml}",
    }

    links
    {
        "Grafika2D",
    }

    defines
    {
    }

    filter "system:windows"
        systemversion "latest"
        entrypoint "mainCRTStartup"
        
    filter "configurations:Debug"
        defines "GRAFIKAen_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "GRAFIKA_RELEASE"
        runtime "Release"
        optimize "on"