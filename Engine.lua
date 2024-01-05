
 project "Engine"
  	  kind "StaticLib"
 	  language "C++"
  	  cppdialect "C++20"
	  targetdir "build/%{cfg.buildcfg}"
	  staticruntime "off"
	  
  	  files
  	  {
	  	"include/**.h",
		"src/**.cpp"
	  }

	  includedirs
	  {
		"include"
	  }

	  targetdir ("../build/bin/" .. OutputDir .. "/%{prj.name}")
   	  objdir ("../build/bin-int/" .. OutputDir .. "/%{prj.name}")

	  filter "system:windows"
	  	 systemversion "latest"
		 defines {}

  	  filter "configurations:Debug"
  	        defines "DEBUG"
	 	runtime "Debug"
	 	symbols "on"
    	 	optimize "on"

	filter "configurations:Release"
	        defines "RELEASE"
		runtime "Release"
	 	optimize "on"

  	filter "configurations:Dist"
  	        defines "DIST"
	 	runtime "Release"
	 	optimize "on" 
