@echo off

@echo .
@echo %TIME%   Build S8-57

call build_device.bat

call build_panel.bat

call build_loader.bat
