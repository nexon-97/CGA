project "Projection"
	kind "WindowedApp"
	location "vs2013"
	debugdir "../.."

	files { "../src/**" }
	links { "Core" }

	configuration "Debug"
		targetname "Projection_d"

	configuration "Release"
		targetname "Projection"
