@echo off

@echo Rebuild All VS    ...  %TIME%

@echo.
@echo Rebuild KornetDDS ...  %TIME%
cd ../KornetDDS/sources/_VS/Generator/
call reBuild.bat
cd ../Panel/
call reBuild.bat
cd ../../../../_common_/

@echo.
@echo Rebuild KornetOSC ...  %TIME%
cd ../KornetOSC/sources/_VS/DeviceOSC/
call reBuild.bat
cd ../PanelOSC/
call reBuild.bat
cd ../../../../_common_/

@echo.
@echo Rebuild S8-53     ...  %TIME%
cd ../S8-53/sources/_VS/Device53/
call reBuild.bat
cd ../Loader53/
call reBuild.bat
cd ../../../../_common_/

@echo.
@echo Rebuild S8-54     ...  %TIME%
cd ../S8-54/sources/_VS/Device54/
call reBuild.bat
cd ../Loader54/
call reBuild.bat
cd ../../../../_common_/

@echo                        %TIME%
