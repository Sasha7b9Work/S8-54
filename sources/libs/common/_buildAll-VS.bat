@echo off

@echo Build All VS    ...  %TIME%

@echo.
@echo Build KornetDDS ...  %TIME%
cd ../KornetDDS/sources/_VS/Generator/
call build.bat
cd ../Panel/
call build.bat
cd ../../../../_common_/

@echo.
@echo Build KornetOSC ...  %TIME%
cd ../KornetOSC/sources/_VS/DeviceOSC/
call build.bat
cd ../PanelOSC/
call build.bat
cd ../../../../_common_/

@echo.
@echo Build S8-53     ...  %TIME%
cd ../S8-53/sources/_VS/Device53/
call build.bat
cd ../Loader53/
call build.bat
cd ../../../../_common_/

@echo.
@echo Build S8-54     ...  %TIME%
cd ../S8-54/sources/_VS/Device54/
call build.bat
cd ../Loader54/
call build.bat
cd ../../../../_common_/

@echo                      %TIME%
