workspace "gperftools_test"
    configurations { "Debug", "Release" }
    platforms { "x64" }
    targetdir "../bin/%{cfg.buildcfg}"
    language "C++"
    flags {
        "C++11",
        "StaticRuntime",
    }
    includedirs {
        "../deps/include/",
    }
    libdirs {
        "../deps/lib_win",
        "../deps/lib_linux",
    }
    filter "configurations:Debug"
        libdirs{
            "../bin/Debug",
        }
    filter "configurations:Release"
        libdirs{
            "../bin/Release",
        }
    filter { }
    
    filter "configurations:Debug"
    defines { "_DEBUG" }
    symbols "On"
    filter "configurations:Release"
    defines { "NDEBUG" }
    optimize "On"
    filter { }
    
    
    if os.is("windows") then
    end
    if os.is("linux") then
        buildoptions { "-fPIC -static-libgcc -static-libstdc++" }
        links{
            "profiler",
            "tcmalloc",
            "pthread",
        }
    end
    
        
project "test"
    kind "ConsoleApp"
    targetname "test"
    defines { "ENABLE_PROFILER" }
    includedirs {
        "../src",
    }
    files {
        "../src/**",
    }
    
