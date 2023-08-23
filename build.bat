@echo off

chcp 65001
if exist "build" rmdir /s /q build
mkdir build
cd build
cmake ..
cmake --build . --config Debug
cd Debug
fairy-chess.exe