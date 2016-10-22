solution "Distracted Processing"
    configurations { "debug", "release" }

    project "Monitor"
        premake.gcc.cc = 'mpic++'
        language "C++"
        kind "ConsoleApp"
        files "src/**.cpp"
        includedirs { "src" }
        buildoptions { "-w -Wall -std=c++11 `pkg-config --cflags mpich`" }
        linkoptions { "-lpthread `pkg-config --libs mpich`" }
        objdir "obj/src"

        configuration { "debug" }
            targetdir "debug"
            flags "Symbols"

        configuration { "release" }
            targetdir "release"
            flags "OptimizeSpeed"

        if _ACTION == "clean" then
            os.rmdir("debug")
            os.rmdir("release")
            os.rmdir("obj")
        end
