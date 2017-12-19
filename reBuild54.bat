@echo off

@echo Rebuild S8-54

c:\Keil_v5\UV4\UV4 -c sources\Device\Device54.uvprojx -j0
c:\Keil_v5\UV4\UV4 -c sources\Loader\Loader54.uvprojx -j0

call build54.bat
