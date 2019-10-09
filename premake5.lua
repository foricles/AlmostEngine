-- The name of your workspace will be used, for example, to name the Visual Studio .sln file generated by Premake.
workspace "AlmostEngine"
architecture "x32"
	
configurations {"Release", "Debug"}

-- We set the location of the files Premake will generate
location "AlmostEngine"

filter "configurations:Debug"
	defines {}
	symbols "On"
	
filter "configurations:Release"
	defines {}
	optimize "On"

---------------------------------------------------------------------------------------------------
	project "AlmostLib"
		kind "StaticLib"
		language "C++"
		
		targetdir ("%{prj.name}/bin/%{cfg.buildcfg}/%{prj.name}") --executable ouput
		objdir ("%{prj.name}/bin-int/%{cfg.buildcfg}/%{prj.name}") --binnary output
		
		files --files in prj
		{
			"src/**.c",
			"src/**.h",
			"src/**.cpp",
			"src/**.hpp",
		}
		
		includedirs --directories
		{
			"thirdparty/"
		}
		
		filter "system:windows"
			cppdialect "C++11"
			staticruntime "On"
			systemversion "latest"
			
			defines
			{
				"GLEW_STATIC",
			}
			
		postbuildcommands
		{
		
		}
	
---------------------------------------------------------------------------------------------------
	project "Sandbox"
		kind "ConsoleApp"
		language "C++"
		
		targetdir ("%{prj.name}/bin/%{cfg.buildcfg}/%{prj.name}") --executable ouput
		objdir ("%{prj.name}/bin-int/%{cfg.buildcfg}/%{prj.name}") --binnary output		
		
		files --files in prj
		{
			"sandbox/**.c",
			"sandbox/**.h",
			"sandbox/**.cpp",
			"sandbox/**.hpp",
		}
		
		includedirs --directories
		{
			"thirdparty/",
			"src/",
		}
		
		links
		{
			"AlmostLib"
		}
		
		filter "system:windows"
			cppdialect "C++11"
			staticruntime "On"
			systemversion "latest"
			
			defines{}