@echo off

@echo Build All

@echo.
cd ..
cd DDS

call buildDDS.bat

@echo.
cd ..
cd OSC

call buildOSC.bat

@echo.
cd ..
cd S8-53

call build53.bat

@echo.
cd ..
cd S8-54

call build54.bat

cd ..
cd _common_
