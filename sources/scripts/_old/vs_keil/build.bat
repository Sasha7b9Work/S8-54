@echo off

@echo.

@echo %TIME%   Building VS-Keil version

MSBuild.exe ..\..\generated\Keil\S8-54.sln -clp:ErrorsOnly;WarningsOnly -nologo /m
set BUILD_STATUS=%ERRORLEVEL%
if not %BUILD_STATUS%==0 goto Failed

goto Success

:Failed
@echo %TIME%   !!!!!!!! ERROR !!!!!!!!!!!!!!!!!!!!!!!!!!!!! Build Failed !!!!!!
goto Exit

:Success
@echo %TIME%   Complete
echo.

:Exit

