@echo off

@echo .
@echo %TIME%   Rebuild S8-57

call rebuild_device.bat

call rebuild_panel.bat

call rebuild_loader.bat
