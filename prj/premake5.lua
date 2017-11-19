workspace "CGA"
	configurations { "Debug", "Release" }
	platforms { "Win32" }
	language "C++"
	
	includedirs
	{
		"../sfml/include",
		".."
	}
	
	libdirs
	{
		"../sfml/lib"
	}
	
	defines { "WIN32", "_WINDOWS", "SFML_STATIC" }
	links { "openal32.lib", "freetype.lib", "jpeg.lib", "opengl32.lib", "winmm.lib", "gdi32.lib" }
	
	configuration "Debug"
		defines { "_DEBUG", "DEBUG", "PROFILE" }
		links { "sfml-main-d.lib", "sfml-window-s-d.lib", "sfml-system-s-d.lib", "sfml-graphics-s-d.lib" }
		targetdir "../bin"
		links {  }
		symbols "on"

	configuration "Release"
		defines { "NDEBUG" }
		links { "sfml-main.lib", "sfml-window-s.lib", "sfml-system-s.lib", "sfml-graphics-s.lib" }
		targetdir "../bin"
		links {  }
		
include "../archimedean_spiral/prj/premake5"
include "../triangle_recursion/prj/premake5"
include "../clipping/prj/premake5"

project "Core"
	kind "StaticLib"
	location "vs2013/core"
	debugdir ".."

	files { "../core/**" }

	configuration "Debug"
		targetname "Core_d"

	configuration "Release"
		targetname "Core"
