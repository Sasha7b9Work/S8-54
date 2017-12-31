@echo off

@echo Rebuild S8-54-Open

c:\Keil_v5\UV4\UV4 -c sources\Device54\Device54-Open7XX.uvprojx -j0
c:\Keil_v5\UV4\UV4 -c sources\Loader54\Loader54-Open7XX.uvprojx -j0

call build54-Open.bat
