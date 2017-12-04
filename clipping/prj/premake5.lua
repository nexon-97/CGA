project "Clipping"
	kind "WindowedApp"
	location "vs2013"
	debugdir "../.."

	files { "../src/**" }
	links { "Core" }

	configuration "Debug"
		targetname "Clipping_d"

	configuration "Release"
		targetname "Clipping"
