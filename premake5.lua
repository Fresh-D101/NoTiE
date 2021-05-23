workspace "NoTiE"
    architecture "x86_64"

    configurations
    {
        "Debug",
        "Release",
        "Final"
    }

    flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["ImGui"] = "NoTiE/vendor/imgui"

--group "Dependencies"
--
--group ""

project "NoTiE"
    location "NoTiE"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("temp/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    defines
    {
        "SFML_STATIC",
        "NOTIE_CORE"
    }

    includedirs 
    {
        "%{prj.name}/vendor/SFML/include",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.ImGui}"
    }

    libdirs
    {
        "%{prj.name}/vendor/SFML/lib"
    }


    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines "NOTIE_DEBUG"
        runtime "Debug"
        symbols "on"

        links
        {
            "sfml-graphics-s-d.lib",
            "sfml-window-s-d.lib",
            "sfml-system-s-d.lib",
            "sfml-main-d.lib",
            "opengl32.lib",
            "winmm.lib",
            "gdi32.lib",
            "freetype.lib",
            "ImGui"
        }

    filter "configurations:Release"
        defines "NOTIE_RELEASE"
        runtime "Release"
        optimize "on"

        links
        {
            "sfml-graphics-s.lib",
            "sfml-window-s.lib",
            "sfml-system-s.lib",
            "sfml-main.lib",
            "opengl32.lib",
            "winmm.lib",
            "gdi32.lib",
            "freetype.lib",
            "ImGui"
        }

    filter "configurations:Final"
        defines "NOTIE_FINAL"
        runtime "Release"
        optimize "on"

        links
        {
            "sfml-graphics-s.lib",
            "sfml-window-s.lib",
            "sfml-system-s.lib",
            "sfml-main.lib",
            "opengl32.lib",
            "winmm.lib",
            "gdi32.lib",
            "freetype.lib",
            "ImGui"
        }

project "NTTP_Game"
        location "NTTP_Game"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"
        staticruntime "on"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("temp/" .. outputdir .. "/%{prj.name}")

        files
        {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp"
        }

        includedirs
        {
            "NoTiE/vendor/spdlog/include",
            "NoTiE/src",
            "NoTiE/vendor"
        }

        links
        {
            "NoTiE"
        }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines "NOTIE_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "NOTIE_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Final"
        defines "NOTIE_FINAL"
        runtime "Release"
        optimize "on"

include "NoTiE/vendor/imgui"