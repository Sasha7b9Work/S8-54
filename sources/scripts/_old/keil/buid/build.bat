@echo off

@echo .
@echo %TIME%   Build S8-54

call build_device.bat

call build_loader.bat
