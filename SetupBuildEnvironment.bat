@echo off
echo Setting up a MinGW/Qt only environment...
echo -- Set QTDIR to F:\Qt\5.4\mingw491_32
set QTDIR=F:\Qt\5.4\mingw491_32
echo -- Set PATH to %QTDIR%\bin
set PATH=%QTDIR%\bin
echo -- Adding C:\Program Files\CMake\bin
set PATH=%PATH%;C:\Program Files\CMake\bin
echo -- Adding F:\Qt\Tools\mingw491_32\bin;F:\Qt\Tools\mingw491_32\lib to PATH
set PATH=%PATH%;F:\Qt\Tools\mingw491_32\bin;F:\Qt\Tools\mingw491_32\lib
echo -- Adding to F:\Qt\Tools\QtCreator\bin
set PATH=%PATH%;F:\Qt\Tools\QtCreator\bin
echo -- Adding %SystemRoot%\System32 to PATH
set PATH=%PATH%;%SystemRoot%\System32

echo -- Adding Wix to path
set PATH=%PATH%;C:\Program Files\WiX Toolset v3.10\bin


echo -- QMAKESPEC set to win32-g++
set QMAKESPEC=win32-g++
cd %USERPROFILE%
echo -- Set MAKEFLAGS to -j %NUMBER_OF_PRCESSORS%
set MAKEFLAGS=-j %NUMBER_OF_PROCESSORS%
