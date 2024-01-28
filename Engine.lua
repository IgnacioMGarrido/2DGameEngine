
 project "Engine"
  	  kind "StaticLib"
 	  language "C++"
  	  cppdialect "C++20"
	  targetdir "build/%{cfg.buildcfg}"
	  staticruntime "off"

	  extDir = {}
  	  extDir["SDL2"] = "3rdParty/SDL2"
  	  -- extDir["Glad"] = "3rdParty/Glad"


  	  files
  	  {
	  	"include/**.h",
		"src/**.cpp"
		-- "%{extDir.Glad}/src/**",
	  }

	  includedirs
	  {
		"include",
		"%{extDir.SDL2}/include",
		-- "%{extDir.Glad}/include",
	  }

	  links
	  {
		"SDL2",
		"SDL2main"
	  }

	  libdirs
	  {
		"%{extDir.SDL2}/lib/x64",
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
    		optimize "off"

	  filter "configurations:Release"
			defines "RELEASE"
			runtime "Release"
			optimize "on"

	  filter "configurations:Dist"
			defines "DIST"
			runtime "Release"
			optimize "on" 
