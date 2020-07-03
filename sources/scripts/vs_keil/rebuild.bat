@echo off

@echo .
@echo %TIME%   Rebuild Win Device

MSBuild.exe ..\..\generated\Keil\Device\Device.vcxproj -t:rebuild -clp:ErrorsOnly;WarningsOnly -nologo
set BUILD_STATUS=%ERRORLEVEL%
if not %BUILD_STATUS%==0 goto Failed

@echo %TIME%   Rebuild Win Panel

MSBuild.exe ..\..\generated\Keil\Panel\Panel.vcxproj -t:rebuild -clp:ErrorsOnly;WarningsOnly -nologo
set BUILD_STATUS=%ERRORLEVEL%
if not %BUILD_STATUS%==0 goto Filed

@echo %TIME%   Rebuild Win DLoader

MSBuild.exe ..\..\generated\Keil\DLoader\DLoader.vcxproj -t:rebuild -clp:ErrorsOnly;WarningsOnly -nologo
set BUILD_STATUS=%ERRORLEVEL%
if %BUILD_STATUS%==0 goto Success


:Failed
echo .
echo .
echo .
echo       !!!!!!!!!!!!!!! Error !!!!!!!!!!!!!!! Build Failed !!!!!!!!!!!!!!!!!!!!
echo .
echo .
echo .
goto Exit

:Success

:Exit

