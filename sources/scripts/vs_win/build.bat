@echo off

@echo .
@echo %TIME%   Building GUI version ...

MSBuild.exe ..\..\generated\Win32\Device\S8-57.vcxproj -clp:ErrorsOnly;WarningsOnly -nologo
set BUILD_STATUS=%ERRORLEVEL%
if %BUILD_STATUS%==0 goto Succsess

:Failed
@echo %TIME%   !!!!!!!!!!!!!!! Error !!!!!!!!!!!!!!! Build Failed !!!!!!!!!!!!!
goto Exit

:Succsess

:Exit

call copy_wxWidgets_dlls.bat

