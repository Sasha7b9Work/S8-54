@echo off

@echo Rebuild All ...       %TIME%

cd ..
cd KornetDDS

call reBuildKornetDDS.bat

cd ..
cd S8-54

call reBuild54.bat

cd ..

cd S8-53

call reBuild53.bat

cd ..
