@echo off
@echo %TIME%   Compile Loader ...

c:\Keil_v5\UV4\UV4 -b ..\..\Loader54\Loader54.uvprojx -j0 -z -o Loader.out
set BUILD_STATUS=%ERRORLEVEL%
if %BUILD_STATUS%==0 goto Exit

echo ERROR!!! Loader    failed!!!

type ..\..\Loader54\Loader.out

:Exit
