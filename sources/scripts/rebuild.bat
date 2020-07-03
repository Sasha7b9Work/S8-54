@echo off

cd keil
call rebuild.bat
cd ..

cd vs_keil
call rebuild.bat
cd..

cd vs_win
call rebuild.bat
cd..
