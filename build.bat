@echo off

IF "%~1" == "" GOTO PrtHelp
IF "%~1" == "/?" GOTO PrtHelp
IF "%~1" == "-?" GOTO PrtHelp
IF "%~1" == "/h" GOTO PrtHelp
IF "%~1" == "-h" GOTO PrtHelp
IF "%~1" == "/help" GOTO PrtHelp
IF "%~1" == "-help" GOTO PrtHelp

IF "%~1" == "compile" GOTO Compile
IF "%~1" == "build" GOTO Compile

@REM vendor\premake\bin\premake5.exe --cc=clang %1
vendor\premake\bin\premake5.exe %1
GOTO Done

:PrtHelp

echo.
echo Usage: %~n0%~x0 [command]
echo.
echo Commands:
echo.
echo   compile       - Compile the project
echo   gmake         - Generate GNU makefiles for POSIX, MinGW, and Cygwin
echo   gmake2        - Generate GNU makefiles for POSIX, MinGW, and Cygwin
echo   xcode4        - Generate Apple Xcode 4 project files
echo   codelite      - Generate CodeLite project files
echo   vs2015        - Generate Visual Studio 2015 project files
echo   vs2017        - Generate Visual Studio 2017 project files
echo   vs2019        - Generate Visual Studio 2019 project files
echo   vs2022        - Generate Visual Studio 2022 project files
echo.
echo   clean         - Clean project files
echo.
echo For additional information, see the Premake User Guide: https://premake.github.io/docs/
echo.

GOTO Done

:Compile

vendor\premake\bin\premake5.exe --cc=clang vs2022

if not defined DevEnvDir (
    call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"
)

set solutionfile="Grafika2D.sln"
@REM msbuild %solutionfile% /t:Build /p:Configuration=Debug
msbuild %solutionfile% /t:Build /p:Configuration=Release
@REM echo "Not supported yet"

:Done