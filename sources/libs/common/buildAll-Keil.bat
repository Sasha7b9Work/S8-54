@echo off

@echo Build All

@echo.
cd ..
cd KornetDDS

call buildKornetDDS.bat

@echo.
cd ..
cd S8-54

call build54.bat

@echo.
cd ..
cd S8-53

call build53.bat

@echo.
cd ..
cd KornetOSC

call buildKornetOSC.bat

cd ..
cd _common_
