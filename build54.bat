@echo off

@echo Build S8-54

echo Compile Loader54 ... %TIME%

c:\Keil_v5\UV4\UV4 -b sources\Loader54\Loader54.uvprojx -j0 -z -o Loader54.out
if ERRORLEVEL 1 goto BadLoader54

echo Compile Device54 ... %TIME%

:FineLoader54
c:\Keil_v5\UV4\UV4 -b sources\Device54\Device54.uvprojx -j0 -z -o Device54.out
if ERRORLEVEL 1 goto BadDevice54

:FineDevice54
goto leave

:BadLoader54
echo ERROR!!! Loader54 failed!!!
"c:\Program Files\Notepad++\notepad++.exe" -multiInst -alwaysOnTop sources\Loader54\Loader54.out -n1000 -x0 -y0
goto FineLoader54

:BadDevice54
echo ERROR!!! Device54 failed!!!
"c:\Program Files\Notepad++\notepad++.exe" -multiInst -alwaysOnTop sources\Device54\Device54.out -n1000 -x0 -y0
goto FineDevice54

:leave
