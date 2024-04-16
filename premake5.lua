outdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

workspace "Grafika2D"
    startproject "MyGame"

    flags
    {
        "MultiProcessorCompile"
    }

    configurations
    {
        "Debug",
        "Release"
    }

    defines
    {
        -- Windows
        "NOMINMAX",
    }

group ""

    include "Grafika2D"
    include "MyGame"

newaction {
    trigger = "clean",
    description = "Remove all binaries and intermediate files, and project files",
    execute = function ()
        print("Removing all binaries and intermediate files")
        os.rmdir('bin')
        os.rmdir('bin-int')
        print("Removing all project files")
        os.remove('**.sln')
        os.remove('**.vcxproj')
        os.remove('**.vcxproj.*')
        -- codelite
        os.remove('**.workspace')
        os.remove('**.userprefs')
        os.remove('**.project')
        os.remove('**.mk')
        os.remove('.codelite') -- Needs to fix it
        -- vscode
        os.remove('**.code-workspace')
        print("Removing Makefile and Makefile.*")
        os.remove('**Makefile')
        os.remove('**Makefile.*')
        os.remove('**.make')
        -- cmake
        os.remove('**CMakeCache.txt')
        os.remove('**CMakeFiles')
        os.remove('**cmake_install.cmake')
        os.remove('**Makefile')
        os.remove('**Makefile.*')
        os.remove('**cmake_install.cmake')
        os.remove('**CMakeLists.txt')
        print("Done")
    end
}
