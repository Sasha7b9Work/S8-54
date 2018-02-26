@echo off

@echo Build All

cd ..
cd KornetDDS

call buildKornetDDS.bat

cd ..
cd S8-54

call build54.bat

cd ..

cd S8-53

call build53.bat

cd ..
