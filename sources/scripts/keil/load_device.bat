@echo off
@echo %TIME%   Load firmware to Device ...

c:\Keil_v5\UV4\UV4 -f ..\..\Device54\Device54.uvprojx -j0 -o ..\scripts\Keil\out.txt
