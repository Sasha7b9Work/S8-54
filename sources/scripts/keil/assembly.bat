echo off

set _COMPILER_=c:\Keil_v5\UV4\UV4
set _RPOJECT_DEVICE_=..\..\Device54\Device54.uvprojx
set _PROJECT_LOADER_=..\..\Loader54\Loader54.uvprojx

rem 1, если требуется очистка
set isClean=0

rem 1, если требуется сборка
set isBuild=0

rem 1, если требуется загрузка
set isLoad=0

set target=""

rem В 1 нужно установить ту цель, над которой идёт работа
set targetDevice=0
set targetLoader=0

if "%1" equ "build"   set isBuild=1 &                  goto CHECK_ON_LOAD
if "%1" equ "rebuild" set isClean=1 & isBuild=1 &      goto CHECK_ON_LOAD
if "%1" equ "load"    set isLoad=1 & set target="%2" & goto LOADING
goto HINT

rem Проверка на то, нужно ли загружать (второй параметр - "load")
:CHECK_ON_LOAD
if "%2" equ "load" set isLoad=1 & set target="%3" & goto TUNE_TARGETS
set target="%2"

:TUNE_TARGETS
rem Здесь уже идёт проверка того, с каким проектом работать
if %target% equ "device" set targetDevice=1 &                      goto CLEANING
if %target% equ "loader" set targetDevice=1 &                      goto CLEANING
if %target% equ "all"    set targetDevice=1 & set targetLoader=1 & goto CLEANING
goto HINT

:CLEANING
if %isClean%==0 goto BUILDING
if %targetDevice%==1 %_COMPILER_% -c%_RPOJECT_DEVICE_% -j0
if %targetLoader%==1 %_COMPILER_% -c%_PROJECT_LOADER_% -j0

:BUILDING
echo point 1
if %isBuild%==0 goto LOADING
echo point 2
echo %targetDevice%
if %targetDevice%==0 goto BUILD_LOADER
echo point 3
%_COMPILER_% -b%_RPOJECT_DEVICE_% -j0 -z -o Device.out
echo point 4
set _BUILD_STATUS_=%ERRORLEVEL%
echo %_BUILD_STATUS_%
if %_BUILD_STATUS_%==0 goto BUILD_LOADER
echo point 5
echo ERROR!!! Build device failed !!!
echo point 6
type ..\..\Device54\Device.out
echo point 7

:BUILD_LOADER
if %targetLoader%==0 goto LOADING
%_COMPILER_% -b%_PROJECT_LOADER_% -j0 -z -o Loader.out
set _BUILD_STATUS_=%ERRORLEVEL%
if %_BUILD_STATUS_%==0 goto LOADING
echo ERROR!!! Build loader failed !!!
type ..\..\Loader54\Loader54.out

:LOADING
if %targetDevice%==0 goto LOAD_LOADER
%_COMPILER_% -f%_PROJECT_DEVICE% -j0 -o

:LOAD_LOADER
if %targetLoader%==0 goto EXIT
%_COMPILER_% -f%_PROJECT_LOADER_% -j0 -o
goto EXIT


:HINT
echo.
echo Using assembly.bat:
echo                    assembly.bat build        [device^|loader^|all]
echo                    assembly.bat rebuild      [device^|loader^|all]
echo                    assembly.bat load         [device^|loader^|all]
echo                    assembly.bat build load   [device^|loader^|all]
echo                    assembly.bat rebuild load [device^|loader^|all]
echo.
goto EXIT

:EXIT
