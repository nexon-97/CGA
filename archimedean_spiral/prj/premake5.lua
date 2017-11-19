project "ArchimedeanSpiral"
	kind "ConsoleApp"
	location "vs2013"
	debugdir "../.."

	files { "../src/**" }
	links { "Core" }

	configuration "Debug"
		targetname "ArchimedeanSpiral_d"

	configuration "Release"
		targetname "ArchimedeanSpiral"
