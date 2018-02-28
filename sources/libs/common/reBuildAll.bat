@echo off

@echo Rebuild All ...       %TIME%

@echo.
cd ..
cd KornetDDS

call reBuildKornetDDS.bat

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
cd KornetOSC

call reBuildKornetOSC.bat

cd ..
cd _common_
