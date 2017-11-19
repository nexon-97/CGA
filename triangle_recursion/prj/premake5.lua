project "TriangleRecursion"
	kind "ConsoleApp"
	location "vs2013"
	debugdir "../.."

	files { "../src/**" }
	links { "Core" }

	configuration "Debug"
		targetname "TriangleRecursion_d"

	configuration "Release"
		targetname "TriangleRecursion"
