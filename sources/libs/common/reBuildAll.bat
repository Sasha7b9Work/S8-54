@echo off

@echo Rebuild All ...       %TIME%

cd ..
cd S8-54

call reBuildAll.bat

cd ..

cd S8-53

call reBuild53.bat

cd ..
