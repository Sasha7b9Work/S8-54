@echo off

rem Использование
rem script "device/panel/all" "build/rebuild" "load/"""
rem script "device/panel/all" "load"
rem script "run"

if "%1"=="run" goto Label_Run
if "%1"=="load" goto Label_Load

rem ------------------------------------------------- Cleanup -------------------------------------------
:Label_Cleanup
if not "%2"=="rebuild" goto Label_Build
if 

rem -------------------------------------------------- Build --------------------------------------------
:Label_Build

rem -------------------------------------------------- Load ---------------------------------------------
:Label_Load


rem -------------------------------------------------- Run ----------------------------------------------
:Label_Run
@echo %TIME%   Start solution
start ../../OSC.uvmpw
goto Exit

rem -------------------------------------------------- Exit ---------------------------------------------
:Exit