@echo off
@echo %TIME%   Compile Device ...

c:\Keil_v5\UV4\UV4 -b ..\..\Device54\Device54.uvprojx -j0 -z -o Device.out
set BUILD_STATUS=%ERRORLEVEL%
if %BUILD_STATUS%==0 goto Exit

echo ERROR!!! Device    failed!!!
type ..\..\Device54\Device.out

:Exit

copy ..\..\Device\S8-54.bin h:\
copy ..\..\Device\S8-54.bin i:\
