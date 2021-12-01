@echo off

if "%2" EQU "" goto LABEL1
if %2==edit goto BUILD_EDIT

goto HINT

:LABEL1

if "%1" EQU "" goto MAKE
if %1==build goto MAKE

goto HINT

:MAKE

@echo.
@echo %TIME%   Making Visual Studio Keil Debug ...

cd ../..
rmdir "generated/Keil" /s /q
cd scripts/vs_keil
@echo on
cmake ../../CMakeLists.txt -B../../generated/Keil -G "Visual Studio 16 2019" -A Win32

@echo off

@echo %TIME%   Complete

if "%1" EQU "" goto EXIT
if %1==build goto BUILD

:BUILD
    call build.bat
    goto EXIT

:HINT
    echo.
    echo Usage:
    echo       make.bat [build [edit]]
    echo.
    goto EXIT

:BUILD_EDIT
    if %1==build (call make.bat build & call edit.bat & goto EXIT)
    goto HINT

:EXIT
