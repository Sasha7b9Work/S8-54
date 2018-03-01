@echo off

@echo Rebuild All ...       %TIME%

@echo.
cd ..
cd DDS

call reBuildDDS.bat

@echo.
cd ..
cd S8-54

call reBuild54.bat

@echo.
cd ..
cd S8-53

call reBuild53.bat

@echo.
cd ..
cd OSC

call reBuildOSC.bat

cd ..
cd _common_
