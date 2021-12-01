@echo off

@echo.
@echo %TIME%   Rebuild VS-Keil version

MSBuild.exe ..\..\generated\Keil\S8-54.sln -t:rebuild -clp:ErrorsOnly;WarningsOnly -nologo /m
set BUILD_STATUS=%ERRORLEVEL%
if not %BUILD_STATUS%==0 goto Failed

goto Success

:Failed
echo.
echo.
echo.
echo       !!!!!!!!!!!!!!! Error !!!!!!!!!!!!!!! Build Failed !!!!!!!!!!!!!!!!!!!!
echo.
echo.
echo.
goto Exit

:Success
echo %TIME%   Complete
echo.

:Exit

