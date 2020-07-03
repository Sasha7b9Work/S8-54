@echo off

@echo .

@echo %TIME%   Building Win32 version

@echo %TIME%   Building Device ...

MSBuild.exe ..\..\generated\Keil\Device\Device.vcxproj -clp:ErrorsOnly;WarningsOnly -nologo
set BUILD_STATUS=%ERRORLEVEL%
if not %BUILD_STATUS%==0 goto Failed

@echo %TIME%   Building Panel ...

MSBuild.exe ..\..\generated\Keil\Panel\Panel.vcxproj -clp:ErrorsOnly;WarningsOnly -nologo
set BUILD_STATUS=%ERRORLEVEL%
if not %BUILD_STATUS%==0 goto Failed

@echo %TIME%   Building DLoader ...

MSBuild.exe ..\..\generated\Keil\DLoader\DLoader.vcxproj -clp:ErrorsOnly;WarningsOnly -nologo
set BUILD_STATUS=%ERRORLEVEL%
if %BUILD_STATUS%==0 goto Success


:Failed
@echo %TIME%   !!!!!!!! ERROR !!!!!!!!!!!!!!!!!!!!!!!!!!!!! Build Failed !!!!!!
goto Exit

:Success

:Exit

