workspace "CGA"
	configurations { "Debug", "Release" }
	platforms { "Win32" }
	language "C++"
	
	includedirs
	{
		"../sdl/include",
		"../math/include",
		".."
	}
	
	libdirs
	{
		"../sdl/lib"
	}
	
	defines { "WIN32", "_WINDOWS" }
	links { "SDL2.lib" }
	
	configuration "Debug"
		defines { "_DEBUG", "DEBUG", "PROFILE" }
		targetdir "../bin"
		links {  }
		symbols "on"

	configuration "Release"
		defines { "NDEBUG" }
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
	links { "GLM" }

	configuration "Debug"
		targetname "Core_d"

	configuration "Release"
		targetname "Core"

project "GLM"
	kind "StaticLib"
	location "vs2013/glm"
	debugdir ".."

	files { "../glm/**.h", "../glm/**.c", "../glm/**.cpp", "../glm/**.hpp", "../glm/**.inl" }

	configuration "Debug"
		targetname "GLM_d"

	configuration "Release"
		targetname "GLM"
