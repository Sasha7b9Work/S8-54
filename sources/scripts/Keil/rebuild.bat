@echo off

@echo .
@echo %TIME%   Rebuild S8-54

call rebuild_device.bat

rem call rebuild_panel.bat

call rebuild_loader.bat
